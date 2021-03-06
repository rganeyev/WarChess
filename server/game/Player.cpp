#include "game/Player.h"
#include "amf/AMFObjectWriter.h"
#include "amf/AMFArrayWriter.h"
#include "util/FixedArray.hpp"
#include "util/String.h"

HANDLE playersFile = NULL;
volatile LONG lastOffset = 0;

String toString(unsigned int i);


Player::Player(DWORD fileOffset) {
	init();

	overlapped.Offset = fileOffset;
}

//default Player initialization
Player::Player(unsigned int id, String password) {
	init();
	this->id = id;
	this->experience = 100;
	this->gold = 100;
}

Player::~Player() {
	DWORD bytesTransfered;
	GetOverlappedResult(playersFile, &overlapped, &bytesTransfered, TRUE);
}

void Player::commit() {
	/*
	DWORD written;
		WriteFile(playersFile, &id, Player_DataSize, &written, &overlapped);
	
		DWORD bytesTransfered;
		GetOverlappedResult(playersFile, &overlapped, &bytesTransfered, TRUE);*/
	

	unlock();
}

void Player::initialize() {
	while (true) {
		DWORD offset = lastOffset;
		if (InterlockedCompareExchange(&lastOffset, offset + Player_DataSize, offset) == offset) {
			overlapped.Offset = offset;
			break;
		}
	}

	commit();
}

void Player::init() {
	memset(this, 0, Player_DataSize); // zero-out player data
}

void Player::lock() {
	//printf("lock %d\n", id);
	rwmutex.lock();
}

void Player::unlock() {
	//printf("unlock %d\n", id);
	rwmutex.unlock();
}

void Player::lock_shared() {
	//printf("lock shared %d\n", id);
	rwmutex.lock_shared();
}

void Player::unlock_shared() {
	//printf("unlock shared %d\n", id);
	rwmutex.unlock_shared();
}

void Player::lock_upgrade() {
	//printf("lock upgrade %d\n", id);
	rwmutex.lock_upgrade();
}

void Player::unlock_upgrade() {
	//printf("unlock upgrade %d\n", id);
	rwmutex.unlock_upgrade();
}

void Player::unlock_upgrade_and_lock() {
	//printf("unlock_upgrade_and_lock %d\n", id);
	rwmutex.unlock_upgrade_and_lock();
}
AMFObjectWriter Player::beginWrite( AMFArrayWriter* writer ) {
	AMFObjectWriter playerWriter = writer->addObject(toString(id));
	playerWriter.begin();
	
	return playerWriter;
}

unsigned int numBitsSet(unsigned int value) {
	unsigned int numBits = 0;
	while (value != 0) {
		value &= (value - 1);
		++numBits;
	}

	return numBits;
}

unsigned int getBitIndex(unsigned int value) {
	__asm {
		mov EAX, value;
		bsr EAX, EAX;
	}
}

void Player::write( AMFArrayWriter* writer) {
	AMFObjectWriter playerWriter = beginWrite(writer);

	writeGold(&playerWriter);
	writeExperience(&playerWriter);

	playerWriter.end();
}


void Player::writeGold( AMFObjectWriter* playerWriter ) {
	playerWriter->writeNumber(String("gold", 4), gold);
}

void Player::writeExperience( AMFObjectWriter* playerWriter )  {
	playerWriter->writeNumber(String("exp", 3), experience);
}

bool Player::addSentChallenge( unsigned int oppId )
{
	lock();
		sentChallenges.insert(oppId);
	unlock();

	char tmp[200];
	AMFWriter streamWriter = AMFWriter(tmp, sizeof(tmp));
	AMFObjectWriter objectWriter = AMFObjectWriter(&streamWriter);
	objectWriter.begin();
		objectWriter.writeNumber(String("id", 2), oppId);
	objectWriter.end();

	return connection->sendRespond(InviteToPlay, &streamWriter);


}

bool Player::addReceivedChallenge( unsigned int oppId )
{
	lock();
		receivedChallenges.insert(oppId);
	unlock();

	char tmp[200];
	AMFWriter streamWriter = AMFWriter(tmp, sizeof(tmp));
	AMFObjectWriter objectWriter = AMFObjectWriter(&streamWriter);
	objectWriter.begin();
		objectWriter.writeNumber(String("id", 2), oppId);
	objectWriter.end();

	return connection->sendRespond(ReceiveInvite, &streamWriter);
}

void Player::removeReceivedChallenge( unsigned int oppId )
{
	lock();
		receivedChallenges.erase(oppId);
	unlock();
}

void Player::removeSentChallenge( unsigned int oppId )
{
	lock();
		sentChallenges.erase(oppId);
	unlock();
}

void Player::clearSentChallenges()
{
	lock();
		sentChallenges.clear();
	unlock();
}
