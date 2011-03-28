#pragma once

#include <Windows.h>

class BaseConnection;

struct AsyncData
{
	AsyncData(bool incoming) : incoming(incoming)
	{
		//memset(&overlapped, 0, sizeof(OVERLAPPED));
		overlapped.hEvent = NULL;
	}

	OVERLAPPED overlapped;
	bool incoming;
};

struct IncomingAsyncData : AsyncData
{
	IncomingAsyncData(BaseConnection* connection) : AsyncData(true), connection(connection) {}
	BaseConnection* connection;
};

struct OutgoingAsyncData : AsyncData
{
	OutgoingAsyncData() : AsyncData(false) {}

	unsigned int messageSize;
	int gameEvent;
	int result;
	char buffer[1];
};