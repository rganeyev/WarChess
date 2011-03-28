#pragma once
#include <assert.h>

template<typename T, unsigned int Capacity>
class Array
{
public:
	Array();

	T& operator[](unsigned int index);
	unsigned int size() const;

	T& insert(unsigned int index, T& value);
	T& incrementSize();

	void eraseFast(unsigned int index);

	unsigned int findById(unsigned int id);
	
	void clear();

private:
	unsigned int _size;
	T _array[Capacity];
};

// represents empty array. It's size is 0 and you can't index into it.
// No operations supported on it. The purpose is generic code allowance.
template<typename T>
class Array<T, 0>
{
public:
	T operator[](unsigned int index) { assert(false); return T(); }
	unsigned int size() const { return 0; }
};