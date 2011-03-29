//#define _DEBUG_

#define _WIN32_WINNT 0x0500
#include <WinSock2.h>
#include "game/PlayerManager.h"
#include "game/Player.h"
#include "util/FixedArray.hpp"
#include "util/Util.h"
#include "util/String.h"

#ifdef _DEBUG_
LONG numCreated = 0;
LONG numDeleted_immediately = 0;
LONG numDeleted_deferred = 0;
LONG numDeleted_failure = 0;

unsigned int lastSize = 0;
#endif // _DEBUG_

PlayerManager* PlayerManager::_instance = NULL;

void CALLBACK _onWriteFinished(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, OVERLAPPED* lpOverlapped) {
	Player* player = (Player*)lpOverlapped;
	player->unlock();
}

PlayerManager::PlayerManager() {
	assert(_instance == NULL);
	_instance = this;

	playersFile = CreateFileA("players.dat", GENERIC_READ | GENERIC_WRITE,
		0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);

	DWORD fileSize = GetFileSize(playersFile, NULL);
	unsigned int numPlayers = fileSize / Player_DataSize;
	
	FixedArray<Player*>* players = FixedArray<Player*>::create(numPlayers + 1);

	for (unsigned int i = 0, offset = 0; i < numPlayers; ++i, offset += Player_DataSize) {
		Player* player = new Player(offset);

		DWORD read;
		BOOL result = ReadFile(playersFile, &player->id, Player_DataSize, &read, &player->overlapped);
		if (result == 0) {
			result = GetOverlappedResult(playersFile, &player->overlapped, &read, TRUE);
		}

		players->set(i, player);
	}

	//my id
	Player* fakePlayer = new Player(3371777, String("PASSWORD", 8));
	players->set(numPlayers, fakePlayer);
	
	//addPlayer(fakePlayer);

	players->sortById();

	_players.rawSet(players);

	BOOL success = BindIoCompletionCallback(playersFile, &_onWriteFinished, 0);
	if (!success) {
		//printError("BindIoCompletionCallback");
	}
}

PlayerManager::~PlayerManager() {
	assert(_instance == this);
	_instance = NULL;
}

Player* PlayerManager::getPlayer(unsigned int id ) {
	FixedArray<Player*>* ptr = _players.addRef();
	unsigned int offset = ptr->findById(id);
	Player* player = (offset == ptr->size()) ? NULL : ptr->get(offset);
	_players.deref(ptr);

	if (player == NULL || player->id != id) {
		return NULL;
	}

	return player;
}

Player* PlayerManager::addPlayer( Player* player ) {
	void* mem = NULL;
	while (true) {
		FixedArray<Player*>* oldPlayers = _players.addRef();
		unsigned int size = oldPlayers->size();
		unsigned int index = oldPlayers->findById(player->id);
		if (index < size) {
			Player* foundPlayer = oldPlayers->get(index);
			if (foundPlayer->id == player->id) {
				return foundPlayer;
			}
		}

		FixedArray<Player*>* newPlayers = oldPlayers->insert(player, index, mem);

		if (newPlayers == NULL) {
			_players.deref(oldPlayers);
			return NULL;
		}

		if (_players.updatePointer(oldPlayers, newPlayers)) {
#ifdef _DEBUG_
			lastSize = size;
			InterlockedIncrement(&numCreated);
#endif // _DEBUG_
			return player;
		}

#ifdef _DEBUG_
		InterlockedIncrement(&numDeleted_failure);
#endif // _DEBUG_

		mem = newPlayers;

		_players.deref(oldPlayers);
	}
}

PlayerManager* PlayerManager::instance() {
	return _instance;
}

bool PlayerManager::addPlayerToOnline(unsigned int id) {
	onlinePlayers.insert(id);
	return true;
}

bool PlayerManager::removePlayerFromOnline(unsigned int id)
{
	onlinePlayers.erase(id);
	return true;
}

std::set<unsigned int> PlayerManager::getOnlinePlayers()
{
	return onlinePlayers;
}
