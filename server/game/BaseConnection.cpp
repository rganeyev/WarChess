#include <new>

#include <game/BaseConnection.h>
#include "util/AsyncData.h"

BaseConnection::BaseConnection( SOCKET socket, unsigned long bufferSize ) : sock(socket) {
	incBuffer.buf = new char[bufferSize];
	incBuffer.len = bufferSize;
}

void BaseConnection::read() {
	DWORD bytesRead = -1;
	DWORD flags = 0;

	void* mem = malloc(sizeof(IncomingAsyncData));
	if (mem == NULL) {
		close();
		return;
	}

	IncomingAsyncData* asyncData = new(mem) IncomingAsyncData(this);
	WSARecv(sock, &incBuffer, 1, &bytesRead, &flags, &asyncData->overlapped, NULL);
}

void BaseConnection::close() {
	closesocket(sock);
}

PolicyConnection::PolicyConnection( SOCKET socket ) : BaseConnection(socket, 64), offset(0) {
	// do nothing
}