#define _WIN32_WINNT 0x0500
#include <WinSock2.h>
#include <Windows.h>

#include "util/Util.h"
#include "util/AsyncData.h"
#include "game/BaseConnection.h"
#include "CrossDomainPolicyResolver.h"

#include <process.h>
#include <assert.h>

extern const char* hostName;

CrossDomainPolicyResolver::CrossDomainPolicyResolver()
{
}

void CrossDomainPolicyResolver::start()
{
	_beginthread(&_startThread, 0, this);
}

void CrossDomainPolicyResolver::_startThread( void* userData )
{
	((CrossDomainPolicyResolver*)userData)->_start();
}

void CrossDomainPolicyResolver::_start()
{
	SOCKET _socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (_socket == INVALID_SOCKET) {  
		printError("socket");
		return;
	}

	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_port = htons(843);

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

		PolicyConnection* connection = new PolicyConnection(incoming);
		connection->read();
	}
}

char crossDomainPolicy[] =	"<?xml version=\"1.0\"?>"
							"<cross-domain-policy>"
							"	<allow-access-from domain=\"*.vk.com\" to-ports=\"3113\" />"
							"<allow-access-from domain=\"*.vkontakte.ru\" to-ports=\"3113\" />"
							"</cross-domain-policy>";

void CALLBACK _onCDIOComplete(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, OVERLAPPED* lpOverlapped)
{
	assert(dwErrorCode == 0);
	AsyncData* data = (AsyncData*)lpOverlapped;
	if (data->incoming) {
		IncomingAsyncData* incomingData = (IncomingAsyncData*)data;
		PolicyConnection* connection = (PolicyConnection*)incomingData->connection;
		if (dwNumberOfBytesTransfered == 0) {
			// connection is closed
			delete connection;
		} else {
			int offset = connection->offset;
			char* str = "<policy-file-request/>" + offset;
			unsigned int length = 23 - offset;
			if (dwNumberOfBytesTransfered <= length) {
				if (memcmp(connection->incBuffer.buf, str, length) == 0) {
					if ((connection->offset += dwNumberOfBytesTransfered) == 23) {
						send(connection->sock, crossDomainPolicy, sizeof(crossDomainPolicy), 0);
					} else if (connection->offset > 23) {
						connection->read();
						return;
					}
				}
			}
 
			connection->close();
			delete connection;
		}
	}

	free(data);
}

void CrossDomainPolicyResolver::_registerIncomingSocket( SOCKET socket )
{
	BOOL success = BindIoCompletionCallback((HANDLE)socket, &_onCDIOComplete, 0);
	if (!success) {
		printError("BindIoCompletionCallback");
	}
}