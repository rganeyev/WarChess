#include "amf/AMFWriter.h"
#include "amf/AMFObjectWriter.h"
#include "amf/AMFArrayWriter.h"
#include "util/String.h"

#include <string.h>

AMFObjectWriter::AMFObjectWriter( AMFWriter* writer ) : _writer(writer)
{
	// do nothing
}

void AMFObjectWriter::begin()
{
	_writer->write<unsigned char>(3);
}

void AMFObjectWriter::writeBoolean( String name, bool value )
{
	_writer->writeUTF(name);
	_writer->write<unsigned char>(1);
	_writer->write<unsigned char>(value);
}

void AMFObjectWriter::writeUTF( String name, String str )
{
	_writer->writeUTF(name);
	_writer->write<unsigned char>(2);
	_writer->writeUTF(str);
}

AMFArrayWriter AMFObjectWriter::writeArray( String name )
{
	_writer->writeUTF(name);
	return AMFArrayWriter(_writer);
}

AMFArrayWriter AMFObjectWriter::writeArray( String prefix, String postfix )
{
	_writer->writeUTF(prefix, postfix);
	return AMFArrayWriter(_writer);
}

AMFObjectWriter AMFObjectWriter::writeObject( String name )
{
	_writer->writeUTF(name);
	return AMFObjectWriter(_writer);
}

void AMFObjectWriter::end()
{
	_writer->write<unsigned char>(0);
	_writer->write<unsigned char>(0);
	_writer->write<unsigned char>(9);
}