#include "game/Connection.h"
#include "game/PlayerManager.h"
#include "game/Player.h"
#include "game/Settings.h"
#include "game/Board.h"

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
	methods[AcceptInvite] = &Connection::do_acceptInvite;
	methods[Move] = &Connection::do_move;
	
}

Connection::~Connection() {
	if (player != NULL) {
		PlayerManager* playerManager = PlayerManager::instance();
		
		playerManager->removePlayerFromOnline(player->id);

		for (std::set<unsigned int>::iterator it = player->sentChallenges.begin(); it != player->sentChallenges.end(); ++it) {
			//connect to opponents and erase
			Player* opponent = playerManager->getPlayer(*it);
			opponent->removeReceivedChallenge(player->id);
		}

		player->clearSentChallenges();

		informPlayersWithOfflinePlayer(player->id);
		//TODO: remove player connection
	}
}

bool Connection::processMessage() {
	unsigned int position = buffer.getReadPosition();
	unsigned int bytesAvailable = buffer.readAvailable();
	printf("received %u bytes \n", bytesAvailable);
	while (true) {
		// 12 = 8 + 4
		if (bytesAvailable < 8) {
			break;
		}

		unsigned int messageLength = buffer.readValue<unsigned int>();
		if (bytesAvailable < messageLength) {
			buffer.setReadPosition(position);
			break;
		}

		if (messageLength < 4) {
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


	AMFReader request = AMFReader(buffer, messageLength);
	enforce(request.readArgName() == String("id", 2));
	unsigned int id = request.readUnsignedInt();
	request.end();

	if (this->player != NULL) {
		return sendError(Auth, PlayerAlreadyAuthorized);
	}

	Player* player = playerManager->getPlayer(id);
	if (player == NULL) {
		return sendError(Auth, PlayerNotFound);
	}

	player->lock();

	if (!player->registrationConfirmed) {
		player->registrationConfirmed = true;
	}

	player->unlock();
	{
		this->player = player;
		player->connection = this;

		//add player to online
		playerManager->addPlayerToOnline(player->id);
		informPlayersWithOnlinePlayer(player->id);


		char tmp[4];
		AMFWriter streamWriter = AMFWriter(tmp, sizeof(tmp));
		return sendRespond(Auth, &streamWriter);
	}
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
	Player* player = new Player(id, String("1", 1, true));
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
	
	AMFReader request = AMFReader(buffer, messageLength);
	request.end();
	
	PlayerManager* playerManager = PlayerManager::instance();
	std::set<unsigned int> online = playerManager->getOnlinePlayers();

	//is that enough?
	char tmp[2000];
	AMFWriter streamWriter = AMFWriter(tmp, sizeof(tmp));
	AMFArrayWriter arrayWriter = AMFArrayWriter(&streamWriter);
	arrayWriter.begin(online.size());
	size_t i = -1;
	for (std::set<unsigned int>::iterator it = online.begin(); it != online.end(); ++it) {
		AMFObjectWriter playerWriter = arrayWriter.addObject(toString(++i));
		playerWriter.begin();
			playerWriter.writeNumber(String("id", 2), *it);
		playerWriter.end();
	}
	arrayWriter.end();

	return sendRespond(GetOnlinePlayers, &streamWriter);
}

bool Connection::do_inviteToPlay( unsigned int messageLength )
{
	AMFReader request = AMFReader(buffer, messageLength);
	enforce(request.readArgName() == String("id", 2));
	unsigned int oppId = request.readUnsignedInt();
	request.end();

	PlayerManager* playerManager = PlayerManager::instance();
	Player* opponent = playerManager->getPlayer(oppId);
	if (opponent == NULL) {
		return sendError(InviteToPlay, PlayerNotFound);
	}
	if (player->sentChallenges.find(oppId) != player->sentChallenges.end()) {
		return sendError(InviteToPlay, PlayerAlreadyChallenged);
	}

	return (player->addSentChallenge(oppId)) && (opponent->addReceivedChallenge(player->id)); 
}

bool Connection::do_acceptInvite( unsigned int messageLength )
{
	//receive id & response
	AMFReader request = AMFReader(buffer, messageLength);
	unsigned int response = 0;
	unsigned int id = 0;
	while (!request.empty()) {
		String argName = request.readArgName();
		if (argName == String("response", 8)) {
			response = request.readUnsignedInt();
		} else if (argName == String("id", 2)) {
			id = request.readUnsignedInt();
		}
	}
	request.end();

	

	//if accepted game
	if (response == 0) {
		PlayerManager::instance()->getPlayer(id)->removeSentChallenge(player->id);
		player->removeReceivedChallenge(id);

		return do_startGame(PlayerManager::instance()->getPlayer(id));
	}
	return sendError(AcceptInvite, RefuseGame);
}

bool Connection::do_startGame(Player* opponent)
{
	char tmp[20000];
	AMFWriter streamWriter = AMFWriter(tmp, sizeof(tmp));
	player->board = new Board(player, opponent, &streamWriter);
	opponent->board = player->board;

	player->opponent = opponent;
	opponent->opponent = player;
	//TODO: delete opponent on game end

	bool result = sendRespond(AcceptInvite, &streamWriter);
	return  result & opponent->connection->sendRespond(AcceptInvite, &streamWriter);
}




Result Connection::informPlayers(GameEvent event, unsigned int id) {
	bool result = true;
	char tmp[200];
	AMFWriter streamWriter = AMFWriter(tmp, sizeof(tmp));
	AMFObjectWriter objectWriter = AMFObjectWriter(&streamWriter);
	objectWriter.begin();
		objectWriter.writeNumber(String("id", 2), id);
	objectWriter.end();

	PlayerManager* playerManager = PlayerManager::instance();
	std::set<unsigned int> online = playerManager->getOnlinePlayers();

	for (std::set<unsigned int>::iterator it = online.begin(); it != online.end(); ++it) {
		if (*it == id) {
			continue;
		}
		result &= (playerManager->getPlayer(*it)->connection->sendRespond(event, &streamWriter));
	}
	if (result) {
		return Success;
	} else {
		return UnknownProblem;
	}
}

//informs other players with new online players
Result Connection::informPlayersWithOnlinePlayer(unsigned int id )
{
	return informPlayers(AddOnlinePlayer, id);
}

Result Connection::informPlayersWithOfflinePlayer( unsigned int id )
{
	return informPlayers(RemoveOnlinePlayer, id);
}

bool Connection::do_move( unsigned int messageLength )
{
	AMFReader request = AMFReader(buffer, messageLength);
	String from;
	String to;
	char fromMove[3];
	char toMove[3];
	while (!request.empty()) {
		String argName = request.readArgName();
		if (argName == String("from", 4)) {
			from = request.readUTF(fromMove, sizeof(fromMove));
		} else if (argName == String("to", 2)) {
			to = request.readUTF(toMove, sizeof(toMove));
		}
	}
	request.end();
	

	bool ret = player->board->move(from.ptr(), to.ptr());
	if (ret) {
		char tmp[200];
		AMFWriter streamWriter = AMFWriter(tmp, sizeof(tmp));
		AMFObjectWriter objectWriter = AMFObjectWriter(&streamWriter);
		objectWriter.begin();
			objectWriter.writeBoolean(String("turn", 4), player->board->getTurn());
			objectWriter.writeUTF(String("from", 4), from);
			objectWriter.writeUTF(String("to", 2), to);
		objectWriter.end();
		return sendRespond(Move, &streamWriter) && player->opponent->connection->sendRespond(Move, &streamWriter);
	} else {
		return sendError(Move, IllegalMove);
	}
}

