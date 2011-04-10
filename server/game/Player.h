#pragma once

#include <WinSock2.h>
#include <Windows.h>
#include "amf/AMFObjectWriter.h"
#include <boost/thread/shared_mutex.hpp>
#include "game/Connection.h"
#include "game/Board.h"
#include <set>

#include <stdio.h>

extern HANDLE playersFile;
extern volatile LONG lastOffset;

class String;
class AMFObjectWriter;
class Connection;
class Board;


struct PlayerData {
	OVERLAPPED overlapped;

	unsigned int id;
	unsigned int experience;
	unsigned int gold;

	unsigned int registrationConfirmed : 1;
	Connection* connection;
	Board* board;
};

class Player : public PlayerData {
private:
	void init(); //memset

public:
	Player(DWORD fileOffset); //add some offset
	Player(unsigned int id, String password); //register player by id
	~Player();

	void initialize();
	void commit();
	bool addSentChallenge(unsigned int oppId);
	bool addReceivedChallenge(unsigned int oppId);
	void removeSentChallenge(unsigned int oppId);
	void removeReceivedChallenge(unsigned int oppId);
	void clearSentChallenges();

	
	void writeGold(AMFObjectWriter* playerWriter);
	void writeExperience(AMFObjectWriter* playerWriter);

	void lock();
	void unlock();
	void lock_shared();
	void unlock_shared();
	void lock_upgrade();
	void unlock_upgrade();
	void unlock_upgrade_and_lock();

	AMFObjectWriter beginWrite(AMFArrayWriter* writer);
	void write(AMFArrayWriter* writer);

	std::set<unsigned int> sentChallenges;
	std::set<unsigned int> receivedChallenges;
	Player* opponent;
private:
	boost::shared_mutex rwmutex;
};

enum {
	Player_DataSize = sizeof(PlayerData) - sizeof(OVERLAPPED),
};