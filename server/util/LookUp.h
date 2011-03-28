#pragma once

template<typename T>
unsigned int getId(T& value)
{
	return value.id;
}

template<typename T>
unsigned int getId(T* value)
{
	return value->id;
}

template<typename T>
struct Comparator
{
	bool operator()(T lhs, T rhs)
	{
		return getId(lhs) < getId(rhs);
	}
};

template<typename T, typename Pred>
unsigned int find(T* array, unsigned int size, Pred pred)
{
	unsigned int lower = 0;
	unsigned int upper = size;

	while (upper > lower)
	{
		unsigned int middle = (lower + upper) / 2;
		if (pred(array[middle])) {
			lower = middle + 1;
		} else {
			upper = middle;
		}
	}

	return lower;
}

template<typename T>
struct IdFinder
{
	IdFinder(unsigned int id) : id(id) {}
	bool operator()(T value)
	{
		return getId(value) < id;
	}

private:
	unsigned int id;
};

struct ValueFinder
{
	ValueFinder(unsigned int value) : value(value) {}
	bool operator()(unsigned int other)
	{
		return other <= value;
	}

private:
	unsigned int value;
};

template<typename T>
unsigned int findById(unsigned int id, T* array, unsigned int size)
{
	return find(array, size, IdFinder<T>(id));
}