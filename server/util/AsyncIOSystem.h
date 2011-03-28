#pragma once

#include <windows.h>

class AsyncIOSystem
{
public:
	AsyncIOSystem(unsigned int numThreads);
	~AsyncIOSystem();

	void registerSocket(SOCKET socket, int key);

private:
	HANDLE _hIOCP;
};