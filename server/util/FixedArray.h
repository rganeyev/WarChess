#pragma once

template<typename T> class AtomicPointer;

template<class T>
class FixedArray
{
protected:
	FixedArray(unsigned int size = 1);

public:
	~FixedArray();

	unsigned int findById(unsigned int id);

	T& get(unsigned int index);
	void set(unsigned int index, T value);
	unsigned int size() const;

	void sortById();

	FixedArray<T>* insert(T value, unsigned int offset, void* mem = NULL);
	FixedArray<T>* remove(unsigned int offset, void* mem = NULL);

	static FixedArray<T>* create(unsigned int size, void* mem = 0);

private:
	unsigned int _size;
	volatile long _counter;
	T _array[1];

	friend class AtomicPointer< FixedArray<T> >;
};