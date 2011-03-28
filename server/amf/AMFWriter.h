#pragma once

#include "util/Enforce.h"
#include "util/ByteOrder.h"

#include <assert.h>

class String;

class AMFWriter
{
public:
	AMFWriter(char* buffer, unsigned int size);

	template<typename T>
	void write(T value)
	{
		unsigned int newOffset = _offset + sizeof(T);
		assert(newOffset <= _size);
		enforce(newOffset <= _size);

		*(T*)(_buffer + _offset) = swapBytes(value);

		_offset = newOffset;
	}

	void write(const char* data, unsigned int dataSize);

	void writeUTF(String str);
	void writeUTF(String prefix, String postfix);

	const char* getPtr();
	unsigned int getSize();

private:
	char*  _buffer;
	unsigned int _size;
	unsigned int _offset;
};