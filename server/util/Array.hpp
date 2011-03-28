#include "util/Array.h"
#include "util/LookUp.h"
#include <assert.h>

template<typename T, unsigned int Capacity>
Array<T, Capacity>::Array() : _size(0)
{
	// do nothing
}

template<typename T, unsigned int Capacity>
T& Array<T, Capacity>::operator []( unsigned int index )
{
	assert(index < _size);
	return _array[index];
}

template<typename T, unsigned int Capacity>
unsigned int Array<T, Capacity>::size() const
{
	return _size;
}

template<typename T, unsigned int Capacity>
unsigned int Array<T, Capacity>::findById( unsigned int id )
{
	return ::findById(id, _array, _size);
}

template<typename T, unsigned int Capacity>
void Array<T, Capacity>::eraseFast( unsigned int index )
{
	assert(index < _size);
	--_size;
	memmove(_array + index, _array + index + 1, (_size - index) * sizeof(T));
}

template<typename T, unsigned int Capacity>
T& Array<T, Capacity>::insert( unsigned int offset, T& value )
{
	memmove(_array + offset + 1, _array + offset, (_size - offset) * sizeof(T));
	_array[offset] = value;

	++_size;

	return _array[offset];
}

template<typename T, unsigned int Capacity>
T& Array<T, Capacity>::incrementSize()
{
	assert(_size < Capacity);
	return _array[_size++];
}

template<typename T, unsigned int Capacity>
void Array<T, Capacity>::clear()
{
	_size = 0;
}