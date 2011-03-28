#pragma once

#include "util/String.h"

class AMFObjectWriter;
class AMFWriter;

class AMFArrayWriter
{
public:
	AMFArrayWriter(AMFWriter* writer);

	void begin(unsigned int numElements);
	AMFObjectWriter addObject(String index);
	void addUnsignedInt(String index, unsigned int value);
	void addBoolean(String index, bool value);
	void end();

private:
	AMFWriter* _writer;
};