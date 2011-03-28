#pragma once

#include "util/ByteOrder.h"

class String;

class CircularBuffer
{
public:
	CircularBuffer();
	~CircularBuffer();

	void write(char* data, unsigned int dataSize);
	void read(char* data, unsigned int dataSize);

	unsigned int size();

	unsigned int readAvailable();	
	unsigned int writeAvailable();

	unsigned int getReadPosition();
	void setReadPosition(unsigned int pos);

	void skipBytes(unsigned int bytes);

	template<typename T>
	T readValue()
	{
		T value;
		read((char*)&value, sizeof(value));

		return swapBytes(value);
	}

	char*  buffer;
	unsigned int bufferSize;
	unsigned int readOffset;
	unsigned int writeOffset;
};

String readUTFBytes(CircularBuffer& buffer, char* tmp, unsigned short len);