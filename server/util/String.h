#pragma once

class String
{
public:
	String();
	String(const char* buffer, unsigned int length, bool check = true);

	bool operator==(String other);
	bool operator!=(String other);

	unsigned int length() const;
	const char* ptr() const;

	bool operator<(String other) const;

private:
	unsigned int _length;
	const char* _buffer;
};