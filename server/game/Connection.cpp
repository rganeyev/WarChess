#include "game/Connection.h"
#include "game/PlayerManager.h"
#include "game/Player.h"
#include "game/Settings.h"

#include "amf/AMFReader.h"
#include "amf/AMFWriter.h"
#include "amf/AMFObjectWriter.h"
#include "amf/AMFArrayWriter.h"
#include "util/FixedArray.hpp"
#include "util/AsyncData.h"
#include "util/Enforce.h"

#include "game/Result.h"

#include <stdlib.h>

time_t now() {
	return time(0);
}

__declspec(thread) char intToStrBuf[8];
String toString(unsigned int i) {
	int len = sprintf(intToStrBuf, "%d", i);
	return String(intToStrBuf, len);
}

Connection::Connection( SOCKET sock ) : BaseConnection(sock, 32*1024) {
	player = NULL;

	//TODO: add all methods

	methods[Register] = &Connection::do_register;
	methods[Auth] = &Connection::do_auth;
	methods[GetOnlinePlayers] = &Connection::do_getOnlinePlayers;
	methods[InviteToPlay] = &Connection::do_inviteToPlay;
}

Connection::~Connection() {
	if (player != NULL) {
		player->lock();
		//TODO: clean challenges


		player->unlock();
		//remove from online
		PlayerManager* playerManager = PlayerManager::instance();
		playerManager->removePlayerFromOnline(player->id);

	}
}

bool Connection::processMessage() {
	unsigned int position = buffer.getReadPosition();
	unsigned int bytesAvailable = buffer.readAvailable();
	printf("received %u bytes \n", bytesAvailable);
	while (true) {
		// 12 = 8 + 4
		if (bytesAvailable < 12) {
			break;
		}

		unsigned int messageLength = buffer.readValue<unsigned int>();
		if (bytesAvailable < messageLength) {
			buffer.setReadPosition(position);
			break;
		}

		if (messageLength < 8) {
			return false;
		} else {
			if (!readMessage(messageLength)) {
				return false;
			}
		}

		bytesAvailable -= messageLength;
		
		if (buffer.readAvailable() != bytesAvailable) {
			return false;
		}
	}

	return true;
}

bool Connection::readMessage( unsigned int length ) {
	GameEvent gameEvent = (GameEvent)buffer.readValue<unsigned int>();
	if (gameEvent >= GameEvent_MAX) {
		return false;
	}

	try {
		length -= 8;
		Method m = methods[gameEvent];
		(this->*m)(length);
	} catch (...) {
		return sendError(gameEvent, IncorrectMessage);
	}

	return true;
}

bool Connection::do_auth( unsigned int messageLength ) {
	PlayerManager* playerManager = PlayerManager::instance();
	char password_pool[PasswordLength];
	String password;
	unsigned int id;

	AMFReader request = AMFReader(buffer, messageLength);
	while (!request.empty()) {
		String argName = request.readArgName();
		if (argName == String("password", 8)) {
			password = request.readUTF(password_pool, sizeof(password_pool));
		} else if (argName == String("id", 2)) {
			id = request.readUnsignedInt();
		}
	}
	request.end();

	if (this->player != NULL) {
		return sendError(Auth, PlayerAlreadyAuthorized);
	}

	Player* player = playerManager->getPlayer(id);
	if (player == NULL) {
		return sendError(Auth, PlayerNotFound);
	}

	Result result;

	player->lock();

	if (password != String(player->password, PasswordLength, false)) {
		result = PasswordIsIncorrect;
		goto error;
	}

	if (!player->registrationConfirmed) {
		player->registrationConfirmed = true;
	}

	player->unlock();
	{
		this->player = player;
		//add player to online
		playerManager->addPlayerToOnline(player->id);

		char tmp[4];
		AMFWriter streamWriter = AMFWriter(tmp, sizeof(tmp));
		/*AMFObjectWriter objectWriter = AMFObjectWriter(&streamWriter);
		objectWriter.begin();
		objectWriter.end();
		*/
		return sendRespond(Auth, &streamWriter);
	}
error:
	player->unlock();
	return sendError(Auth, result);
}


bool Connection::do_register( unsigned int messageLength ) {
	PlayerManager* playerManager = PlayerManager::instance();

	AMFReader request = AMFReader(buffer, messageLength);
	enforce(request.readArgName() == String("id", 2));
	unsigned int id = request.readUnsignedInt();
	request.end();

	Player* player;
	Result result = createNewPlayer(id, &player);
	if (result != Success) {
		// failed to register player for some reason
		return sendError(Register, result);
	}

	char tmp[200];
	AMFWriter streamWriter = AMFWriter(tmp, sizeof(tmp));
	AMFObjectWriter playerWriter = AMFObjectWriter(&streamWriter);
	playerWriter.begin();
		playerWriter.writeUTF(String("password", 8), String(player->password, PasswordLength, false));
	playerWriter.end();

	return sendRespond(Register, &streamWriter);
}

bool Connection::sendRespond( GameEvent gameEvent, AMFWriter* writer ) {
	send(sock, gameEvent, Success, writer->getPtr(), writer->getSize());
	return true;
}

bool Connection::sendError( GameEvent gameEvent, int errorCode ) {
	send(sock, gameEvent, errorCode, NULL, 0);
	return true;
}

void Connection::send( SOCKET sock, GameEvent gameEvent, int result, const char* data, unsigned int dataSize ) {
	void* mem = malloc(dataSize + sizeof(OutgoingAsyncData) - 1);

	OutgoingAsyncData* asyncData = new(mem) OutgoingAsyncData();

	unsigned int messageSize = dataSize + 12;

	printf("sent %u bytes\n", messageSize);
	asyncData->messageSize = htonl(messageSize);
	asyncData->gameEvent = htonl(gameEvent);
	asyncData->result = htonl(result);

	memcpy(asyncData->buffer, data, dataSize);

	WSABUF buf;
	buf.buf = (char*)&asyncData->messageSize;
	buf.len = messageSize;

	DWORD sent;
	WSASend(sock, &buf, 1, &sent, 0, &asyncData->overlapped, NULL);
}

void Connection::skipMessage( unsigned int length ) {
	buffer.setReadPosition(buffer.getReadPosition() + length);
}

Result Connection::createNewPlayer( unsigned int id, Player** pPlayer ) {
	char password[PasswordLength] = { 'P', 'A', 'S', 'S', 'W', 'O', 'R', 'D' };
	// a code to generate random one
	/*
	for (int i = 0; i < PasswordLength; ++i) {
			password[i] = 33 + (rand()%94);
		}*/
	

	Player* player = new Player(id, String(password, PasswordLength, false));
	player->lock();

	PlayerManager* playerManager = PlayerManager::instance();
	Player* foundPlayer = playerManager->addPlayer(player);

	Result result;

	if (player == foundPlayer) {
		player->initialize();
		result = Success;
	} else {
		if (foundPlayer == NULL) {
			result = OutOfMemory;
		} else {
			assert(player != foundPlayer);
			player->unlock();
			delete player;

			foundPlayer->lock();
			if (foundPlayer->registrationConfirmed) {
				result = PlayerAlreadyExists;
				foundPlayer->unlock();
			} else {
				result = Success;
				foundPlayer->commit();
			}
		}
	}

	*pPlayer = foundPlayer;
	return result;
}

bool Connection::do_getOnlinePlayers( unsigned int messageLength )
{
	PlayerManager* playerManager = PlayerManager::instance();
	std::set<unsigned int> online = playerManager->getOnlinePlayers();

	//is that enough?
	char tmp[200];

	AMFWriter streamWriter = AMFWriter(tmp, sizeof(tmp));
	AMFArrayWriter arrayWriter = AMFArrayWriter(&streamWriter);
	arrayWriter.begin(online.size());
	size_t i = -1;
	for (std::set<unsigned int>::iterator it = online.begin(); it != online.end(); ++it) {
		AMFObjectWriter playerWriter = arrayWriter.addObject(toString(++i));
			playerWriter.begin();
				playerWriter.writeNumber(String("id", 2), *it);
			playerWriter.end();
		arrayWriter.end();
	}

	return sendRespond(GetOnlinePlayers, &streamWriter);
}

bool Connection::do_inviteToPlay( unsigned int messageLength )
{
	AMFReader request = AMFReader(buffer, messageLength);
	enforce(request.readArgName() == String("id", 2));
	unsigned int oppId = request.readUnsignedInt();
	PlayerManager* playerManager = PlayerManager::instance();
	Player* opponent = playerManager->getPlayer(oppId);
	if (opponent == NULL) {
		return sendError(InviteToPlay, PlayerNotFound);
	}
	player->addSentChallenge(oppId);
	
	char tmp[200];
	AMFWriter streamWriter = AMFWriter(tmp, sizeof(tmp));
	AMFObjectWriter objectWriter = AMFObjectWriter(&streamWriter);
	objectWriter.begin();
		objectWriter.writeNumber(String("id", 2), oppId);
	objectWriter.end();

	char tmp2[200];
	opponent->addReceivedChallenge(player->id);
	AMFWriter streamWriterForInvite = AMFWriter(tmp2, sizeof(tmp2));
	objectWriter = AMFObjectWriter(&streamWriterForInvite);
	objectWriter.begin();
		objectWriter.writeNumber(String("id", 2), player->id);
	objectWriter.end();

	return sendRespond(InviteToPlay, &streamWriter) && sendRespond(ReceiveInvite, & streamWriterForInvite);
}

