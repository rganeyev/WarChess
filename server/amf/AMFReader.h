#pragma once

#include "util/String.h"
#include "util/Enforce.h"

class CircularBuffer;

class AMFReader
{
public:
	AMFReader(CircularBuffer& buffer, unsigned int objectSize);

	bool empty();
	String readArgName();
	unsigned int readUnsignedInt();
	String readUTF(char* tmp, unsigned short tmpLen);
	void end();

private:
	CircularBuffer& buffer;
	char pool[16];
	unsigned int endBufferSize;
};