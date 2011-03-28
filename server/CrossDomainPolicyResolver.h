#pragma once

class CrossDomainPolicyResolver
{
public:
	CrossDomainPolicyResolver();
	void start();

private:
	static void _startThread(void* userData);
	void _start();
	void _registerIncomingSocket(SOCKET socket);
};