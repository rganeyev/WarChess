#pragma once

#include "game/GameEvent.h"
#include "game/BaseConnection.h"
#include "util/CircularBuffer.h"
#include <boost/thread/shared_mutex.hpp>
#include <WinSock2.h>
#include <vector>

class AMFWriter;
class Player;

enum Result;

class Connection : public BaseConnection
{
public:
	Connection(SOCKET sock);
	~Connection();

	bool processMessage();

	bool do_register(unsigned int messageLength);
	bool do_auth(unsigned int messageLength);
	bool do_getOnlinePlayers(unsigned int messageLength);
	bool do_inviteToPlay(unsigned int messageLength);
	bool do_receiveInvite(unsigned int messageLength);
	bool do_acceptInvite(unsigned int messageLength);
	
	Result createNewPlayer(unsigned int id, Player** pPlayer);
	//TODO: have this methods implemented
	/*
		bool do_sendPlayerDetails();
		bool do_receiveInvite();
		bool do_acceptInvite();
		bool do_refuseInvite();
		bool do_move();
		bool do_getState();*/
	


	bool readMessage(unsigned int length);
	void skipMessage(unsigned int length);

	bool sendError(GameEvent gameEvent, int errorCode);
	bool sendRespond(GameEvent gameEvent, AMFWriter* writer);

	static void send(SOCKET socket, GameEvent gameEvent, int result, const char* data, unsigned int size);

	CircularBuffer buffer;
	Player* player;

	typedef bool (__thiscall Connection::* Method)(unsigned int messageLength);
	Method methods[GameEvent_MAX];
};