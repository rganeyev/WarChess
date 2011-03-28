#pragma once

#include "amf/AMFWriter.h"

class AMFArrayWriter;
class String;

class AMFObjectWriter
{
public:
	AMFObjectWriter(AMFWriter* writer);

	void begin();

	template<typename T>
	void writeNumber(String name, T value)
	{
		_writer->writeUTF(name);
		_writer->write<unsigned char>(0);
		_writer->write<double>((double)value);
	}

	void writeBoolean(String name, bool value);
	void writeUTF(String name, String str);
	AMFArrayWriter writeArray(String name);
	AMFArrayWriter writeArray(String prefix, String postfix);
	AMFObjectWriter writeObject(String name);

	void end();

private:
	AMFWriter* _writer;
};