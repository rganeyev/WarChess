#include "amf/AMFWriter.h"
#include "amf/AMFArrayWriter.h"
#include "amf/AMFObjectWriter.h"

AMFArrayWriter::AMFArrayWriter( AMFWriter* writer ) : _writer(writer)
{
	// do nothing
}

void AMFArrayWriter::begin(unsigned int numElements)
{
	_writer->write<unsigned char>(8);
	_writer->write<unsigned int>(numElements);
}

AMFObjectWriter AMFArrayWriter::addObject( String index )
{
	_writer->writeUTF(index);
	return AMFObjectWriter(_writer);
}

void AMFArrayWriter::end()
{
	_writer->write<unsigned char>(0);
	_writer->write<unsigned char>(0);
	_writer->write<unsigned char>(9);
}

void AMFArrayWriter::addUnsignedInt( String index, unsigned int value )
{
	_writer->writeUTF(index);
	_writer->write<unsigned char>(0);
	_writer->write<double>((double)value);
}

void AMFArrayWriter::addBoolean( String index, bool value )
{
	_writer->writeUTF(index);
	_writer->write<unsigned char>(1);
	_writer->write<unsigned char>(value);
}

void AMFArrayWriter::addString( String index, String value )
{
	_writer->writeUTF(index);
	_writer->write<unsigned char>(2);
	_writer->writeUTF(value);
}
