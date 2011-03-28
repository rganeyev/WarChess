#include "util/AsyncIOSystem.h"
#include "util/Util.h"
#include <assert.h>

AsyncIOSystem::AsyncIOSystem(unsigned int numThreads)
{
	assert(numThreads != 0);

	_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	Assert(_hIOCP != INVALID_HANDLE_VALUE, "CreateIoCompletionPort");
	Assert(_hIOCP != NULL, "CreateIoCompletionPort");

	/*
	_workingThreads = new IOCPWorkingThread[numThreads];
	for (int i = 0; i < maxThreads; ++i) {
		IOCPWorkingThread thread = _workingThreads[i] = new IOCPWorkingThread(_hIOCP);
		thread.start();
	}
	*/
}

AsyncIOSystem::~AsyncIOSystem()
{
	CloseHandle(_hIOCP);
}

void AsyncIOSystem::registerSocket(SOCKET socket, int key)
{
	HANDLE h = CreateIoCompletionPort((HANDLE)socket, _hIOCP, key, 0);
	Assert(h != NULL, "CreateIoCompletionPort");
}