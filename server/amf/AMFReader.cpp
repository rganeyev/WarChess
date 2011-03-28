#include "amf/AMFReader.h"
#include "util/CircularBuffer.h"

AMFReader::AMFReader( CircularBuffer& buffer, unsigned int objectSize ) : buffer(buffer)
{
	unsigned int available = buffer.readAvailable();
	enforce(available >= objectSize);
	endBufferSize = available - objectSize;

	unsigned char mark = buffer.readValue<unsigned char>();
#ifdef _DEBUG
	enforce(mark == 3);
#endif
}

bool AMFReader::empty()
{
	return (buffer.readAvailable() == endBufferSize + 3);
}

String AMFReader::readArgName()
{
	unsigned short len = buffer.readValue<unsigned short>();
	enforce(len <= sizeof(pool));

	return readUTFBytes(buffer, pool, len);
}

unsigned int AMFReader::readUnsignedInt()
{
	unsigned char mark = buffer.readValue<unsigned char>();
	enforce(mark == 0);

	return (unsigned int)buffer.readValue<double>();
}

String AMFReader::readUTF( char* tmp, unsigned short tmpLen )
{
	unsigned char mark = buffer.readValue<unsigned char>();
	if (mark == 6) {
		return String("undefined", 9);
	}

	enforce(mark == 2);

	unsigned short len = buffer.readValue<unsigned short>();
	enforce(len <= tmpLen);

	buffer.read(tmp, len);

	return String(tmp, len, false);
}

void AMFReader::end()
{
	enforce(empty());

#ifdef _DEBUG
	char sign[3];
	buffer.read(sign, 3);

	char sign_expected[3] = {0, 0, 9};
	enforce(memcmp(sign, sign_expected, 3) == 0);

	enforce(buffer.readAvailable() == endBufferSize);
#else
	buffer.skipBytes(3);
#endif
}