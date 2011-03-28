#pragma once
#include <WinSock2.h>

class BaseConnection {
public:
	BaseConnection(SOCKET socket, unsigned long bufferSize);

	void read();
	void close();

	SOCKET sock;
	WSABUF incBuffer;
};

class PolicyConnection : public BaseConnection {
public:
	PolicyConnection(SOCKET socket);
	int offset;
};