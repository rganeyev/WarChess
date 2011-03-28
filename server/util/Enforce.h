#pragma once

inline void enforce(bool condition, const char* errorString = "Unknown internal error")
{
	if (!condition) {
		throw errorString;
	}
}