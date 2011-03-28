#pragma once

#include <WinSock2.h>
#include <algorithm>

inline unsigned int swapBytes(unsigned int value)
{
	return htonl(value);
}

inline unsigned short swapBytes(unsigned short value)
{
	return htons(value);
}

inline unsigned char swapBytes(unsigned char value)
{
	return value;
}

inline double swapBytes(double value)
{
	union Tmp {
		double d;
		int i[2];
	} tmp;

	tmp.d = value;

	int i1 = htonl(tmp.i[0]);
	tmp.i[0] = htonl(tmp.i[1]);
	tmp.i[1] = i1;

	return tmp.d;
}