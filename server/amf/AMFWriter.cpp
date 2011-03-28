#include "amf/AMFWriter.h"
#include "util/String.h"

AMFWriter::AMFWriter( char* buffer, unsigned int size ) : _buffer(buffer), _size(size), _offset(0)
{
	// do nothing
}

const char* AMFWriter::getPtr()
{
	return _buffer;
}

unsigned int AMFWriter::getSize()
{
	return _offset;
}

void AMFWriter::write( const char* data, unsigned int dataSize )
{
	unsigned int newOffset = _offset + dataSize;
	assert(newOffset <= _size);
	enforce(newOffset <= _size);

	memcpy(_buffer + _offset, data, dataSize);

	_offset = newOffset;
}

void AMFWriter::writeUTF( String str )
{
	unsigned short length = (unsigned short)str.length();
	write<unsigned short>(length);
	write(str.ptr(), length);
}

void AMFWriter::writeUTF( String prefix, String postfix )
{
	unsigned short length = (unsigned short)(prefix.length() + postfix.length());
	write<unsigned short>(length);
	write(prefix.ptr(), prefix.length());
	write(postfix.ptr(), postfix.length());
}