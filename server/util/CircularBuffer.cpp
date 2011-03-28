#include "util/CircularBuffer.h"
#include "util/Enforce.h"
#include "util/String.h"

#include <string.h>
#include <assert.h>
#include <malloc.h>

CircularBuffer::CircularBuffer()
{
	readOffset = 0;
	writeOffset = 0;
	bufferSize = 4096;
	buffer = (char*)malloc(bufferSize);
	enforce(buffer != NULL, "Out of memory");
}

CircularBuffer::~CircularBuffer()
{
	free(buffer);
}

void CircularBuffer::write( char* data, unsigned int dataSize )
{
	unsigned int freeSpace = writeAvailable();
	if (freeSpace < dataSize) {
		// reallocate
		unsigned int newBufferSize = (bufferSize - freeSpace + dataSize + 4095) & ~4095;

		char* newBuffer = (char*)malloc(newBufferSize);
		enforce(newBuffer != NULL, "Out of memory");
		if (writeOffset >= readOffset) {
			memcpy(newBuffer, buffer + readOffset, writeOffset - readOffset);
		} else {
			unsigned int beforeSize = bufferSize - readOffset;
			memcpy(newBuffer, buffer + readOffset, beforeSize);
			memcpy(newBuffer+beforeSize, buffer, writeOffset);
		}

		free(buffer);

		buffer = newBuffer;
		bufferSize = newBufferSize;
	}

	assert(writeAvailable() >= dataSize);

	unsigned int newWriteOffset = writeOffset + dataSize;
	if (newWriteOffset <= bufferSize) {
		memcpy(buffer + writeOffset, data, dataSize);
		writeOffset = newWriteOffset;
	} else {
		unsigned int spaceBefore = bufferSize - writeOffset;
		memcpy(buffer + writeOffset, data, spaceBefore);
		writeOffset = dataSize - spaceBefore;
		memcpy(buffer, data + spaceBefore, writeOffset);
	}
}

void CircularBuffer::read( char* data, unsigned int dataSize )
{
	enforce(readAvailable() >= dataSize, "Buffer exhausted");

	unsigned int newReadOffset = readOffset + dataSize;
	if (newReadOffset <= bufferSize) {
		memcpy(data, buffer + readOffset, dataSize);
		readOffset = newReadOffset;
	} else {
		unsigned int spaceBefore = bufferSize - readOffset;
		memcpy(data, buffer + readOffset, spaceBefore);
		readOffset = dataSize - spaceBefore;
		memcpy(data + spaceBefore, buffer, readOffset);
	}
}

unsigned int CircularBuffer::readAvailable()
{
	if (writeOffset >= readOffset) {
		return writeOffset - readOffset;
	}

	return writeOffset + (bufferSize - readOffset);
}

unsigned int CircularBuffer::writeAvailable()
{
	if (readOffset > writeOffset) {
		return readOffset - writeOffset;
	}

	return readOffset + (bufferSize - writeOffset);
}

unsigned int CircularBuffer::getReadPosition()
{
	return readOffset;
}

void CircularBuffer::setReadPosition( unsigned int pos )
{
	readOffset = pos % bufferSize;
}

void CircularBuffer::skipBytes( unsigned int bytes )
{
	enforce(readAvailable() >= bytes, "Buffer exhausted");

	readOffset = (readOffset + bytes) % bufferSize;
}

unsigned int CircularBuffer::size()
{
	return bufferSize;
}

String readUTFBytes( CircularBuffer& buffer, char* tmp, unsigned short len )
{
	buffer.read(tmp, len);
	return String(tmp, len, false);
}