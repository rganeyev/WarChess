#include "Server.h"
#include "game/Connection.h"
#include "util/Util.h"
#include "util/AsyncData.h"

#include <assert.h>
#include <stdio.h>
#include <process.h>

uintptr_t serverThread;

Server::Server()
{
	WSADATA data;
	int result = WSAStartup(MAKEWORD(2,2), &data);
	if (result != NO_ERROR) {
		printError("WSAStartup");
	}

	_socket = INVALID_SOCKET;
}

Server::~Server()
{
	if (_socket != INVALID_SOCKET) {
		closesocket(_socket);
	}

	WSACleanup();
}

void Server::start(unsigned short port)
{
	_port = port;
	serverThread = _beginthread(&_startThread, 0, this);
}

void CALLBACK _onIOComplete(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, OVERLAPPED* lpOverlapped)
{
	//assert(dwErrorCode == 0);
	//printf("Async I/O Complete! dwErrorCode: %d, dwNumberOfBytesTransfered: %d\n", dwErrorCode, dwNumberOfBytesTransfered);

	AsyncData* data = (AsyncData*)lpOverlapped;
	if (data->incoming) {
		IncomingAsyncData* incomingData = (IncomingAsyncData*)data;
		Connection* connection = (Connection*)incomingData->connection;
		if (dwNumberOfBytesTransfered == 0) {
			// connection is closed
			printf("Connection has been closed\n");
			delete connection;
		} else {
			connection->buffer.write(connection->incBuffer.buf, dwNumberOfBytesTransfered);
			if (connection->processMessage()) {
				connection->read();
			} else {
				connection->close();
				delete connection;
			}
		}
	}

	free(data);
}

void Server::_registerIncomingSocket( SOCKET socket )
{
	BOOL success = BindIoCompletionCallback((HANDLE)socket, &_onIOComplete, 0);
	if (!success) {
		printError("BindIoCompletionCallback");
	}
}

extern const char* hostName;

void Server::_start()
{
	_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (_socket == INVALID_SOCKET) {  
		printError("socket");
		return;
	}

	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_port = htons(_port);

	hostent* thisHost = gethostbyname(hostName);
	char* ip = inet_ntoa (*(struct in_addr *)*thisHost->h_addr_list);
	service.sin_addr.s_addr = inet_addr(ip);

	int error = bind(_socket, (SOCKADDR*)&service, sizeof(service));
	if (error == SOCKET_ERROR) {  
		printError("bind");
		return;
	}

	/* Make the socket listen for incoming connection requests */
	error = listen(_socket, 1);
	if (error == SOCKET_ERROR) {  
		printError("listen");
		return;
	}

	while (true)
	{
		sockaddr_in client;
		int clientSize = sizeof(client);
		SOCKET incoming = WSAAccept(_socket,(SOCKADDR*)&client, &clientSize, NULL, NULL);

		_registerIncomingSocket(incoming);

		Connection* connection = new Connection(incoming);
		connection->read();
	}
}

void Server::_startThread( void* userData )
{
	((Server*)userData)->_start();
}