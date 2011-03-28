#include "util/String.h"

#include <string.h>
#include <assert.h>

String::String()
{
	// do nothing
}

String::String( const char* buffer, unsigned int length, bool check ) : _buffer(buffer), _length(length)
{
	assert(!check || (buffer[length] == 0));
}

bool String::operator==( String other )
{
	return (_length == other._length) && (memcmp(_buffer, other._buffer, _length) == 0);
}

bool String::operator!=( String other )
{
	return (_length != other._length) || (memcmp(_buffer, other._buffer, _length) != 0);
}

unsigned int String::length() const
{
	return _length;
}

const char* String::ptr() const
{
	return _buffer;
}

bool String::operator<( String other ) const
{
	if (_length < other._length) return true;
	if (_length > other._length) return false;

	return memcmp(_buffer, other._buffer, _length) < 0;
}