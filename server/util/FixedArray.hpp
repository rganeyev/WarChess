#include "util/FixedArray.h"
#include "util/LookUp.h"

#include <Windows.h>

#include <stdio.h>
#include <new>

template<class T>
FixedArray<T>::FixedArray( unsigned int size ) : _size(size), _counter(0)
{
	// do nothing
}

template<class T>
FixedArray<T>::~FixedArray()
{
	free(this);
}

template<class T>
T& FixedArray<T>::get( unsigned int index )
{
	assert(index < _size);
	return _array[index];
}

template<class T>
void FixedArray<T>::set( unsigned int index, T value )
{
	assert(index < _size);
	_array[index] = value;
}

template<class T>
unsigned int FixedArray<T>::size() const
{
	return _size;
}

template<class T>
unsigned int FixedArray<T>::findById( unsigned int id )
{
	return ::findById(id, _array, _size);
}

template<class T>
FixedArray<T>* FixedArray<T>::insert( T value, unsigned int offset, void* mem )
{
	unsigned int size = _size;

	FixedArray<T>* copy = create(size + 1, mem);
	if (copy == NULL) return NULL;

	memcpy(copy->_array, _array, offset * sizeof(T));
	copy->_array[offset] = value;
	memcpy(copy->_array + offset + 1, _array + offset, (size - offset) * sizeof(T));

	return copy;
}

template<class T>
FixedArray<T>* FixedArray<T>::create( unsigned int size, void* mem /*= 0*/ )
{
	unsigned int numBytes = sizeof(FixedArray<T>) + (size - 1) * sizeof(T);
	mem = realloc(mem, numBytes);

	if (mem == NULL) {
		printf("OutOfMemory: failed to allocate %d bytes\n", numBytes);
		return NULL;
	}

	return new(mem) FixedArray<T>(size);
}

template<class T>
void FixedArray<T>::sortById()
{
	std::sort(_array, _array + _size, Comparator<T>());
}

template<class T>
FixedArray<T>* FixedArray<T>::remove( unsigned int offset, void* mem )
{
	unsigned int size = _size;

	FixedArray<T>* copy = create(size - 1, mem);
	if (copy == NULL) return NULL;

	memcpy(copy->_array, _array, offset * sizeof(T));
	memcpy(copy->_array + offset, _array + offset + 1, (size - offset - 1) * sizeof(T));

	return copy;
}