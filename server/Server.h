#pragma once

#define _WIN32_WINNT 0x0500
#include <WinSock2.h>
#include <Windows.h>

class Server
{
public:
	Server();
	~Server();

	void start(unsigned short port);

private:
	void _registerIncomingSocket(SOCKET socket);
	static void _startThread(void* serverPtr);
	void _start();

	unsigned short _port;
	SOCKET _socket;
};