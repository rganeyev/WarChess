#include "util/Util.h"
#include <stdio.h>

char* sysErrorString(unsigned int errcode)
{
    char* buffer;

    DWORD r = FormatMessageA( 
	    FORMAT_MESSAGE_ALLOCATE_BUFFER | 
	    FORMAT_MESSAGE_FROM_SYSTEM | 
	    FORMAT_MESSAGE_IGNORE_INSERTS,
	    NULL,
	    errcode,
	    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
	    (LPSTR)&buffer,
	    0,
	    NULL);

	if (buffer == 0) {
		char* str = "Unknown error";
		size_t len = strlen(str) + 1;
		char* copy = (char*)LocalAlloc(0, len);
		memcpy(copy, str, len);
		return copy;
	}

    /* Create 0 terminated copy on GC heap because fromMBSz()
     * may return it.
     */
	CharToOemA(buffer, buffer);
	buffer[r-2] = 0;

	return buffer;
}

void printError(char* functionName, int errCode)
{
	char* error = sysErrorString(errCode);
	printf("error at %s: %s (%d/0x%08X)\n", functionName, error, errCode, errCode);
	LocalFree(error);
}