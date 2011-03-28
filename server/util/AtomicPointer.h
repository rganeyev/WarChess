#pragma once

#include <Windows.h>
#include <assert.h>

#ifdef _DEBUG_
extern LONG numDeleted_immediately;
extern LONG numDeleted_deferred;
#endif // _DEBUG_

#if _WIN32_WINNT <= 0x0501
#pragma warning(push) 
#pragma warning(disable : 4035) // disable no-return warning 

inline LONG64 InterlockedCompareExchange64(volatile LONG64* dest, LONG64 exchange, LONG64 comperand)
{ 
	//value returned in eax::edx 
	__asm { 
		lea esi,comperand; 
		lea edi,exchange; 

		mov eax,[esi]; 
		mov edx,4[esi]; 
		mov ebx,[edi]; 
		mov ecx,4[edi]; 
		mov esi,dest; 
		//lock CMPXCHG8B [esi] is equivalent to the following except 
		//that it's atomic: 
		//ZeroFlag = (edx:eax == *esi); 
		//if (ZeroFlag) *esi = ecx:ebx; 
		//else edx:eax = *esi; 
		lock CMPXCHG8B [esi]; 
	} 

} 

#pragma warning(pop)
#endif // InterlockedCompareExchange64

template<class T>
class AtomicPointer
{
public:
	AtomicPointer()
	{
		_value.ptr = NULL;
		_value.counter = 0;
	}

	~AtomicPointer()
	{
		assert(_value.ptr != NULL);
		assert(_value.counter == 1);

		free(_value.ptr);
	}

	void rawSet(T* ptr)
	{
		_value.ptr = ptr;
		_value.counter = 1;
	}

	T* addRef()
	{
		while (true) {
			Value oldValue = _value;
			Value newValue = oldValue;
			++newValue.counter;

			if (InterlockedCompareExchange64(&_value.as64, newValue.as64, oldValue.as64) != oldValue.as64) {
				continue;
			}

			return oldValue.ptr;
		}
	}

	void derefOld(T* ptr, int numRef)
	{
		if (InterlockedExchangeAdd(&ptr->_counter, -numRef) == numRef) {
#ifdef _DEBUG_
			InterlockedIncrement(&numDeleted_deferred);
#endif // _DEBUG_
			free(ptr);
		}
	}

	void deref(T* ptr)
	{
		while (true) {
			Value oldValue = _value;
			if (oldValue.ptr == ptr) {
				Value newValue = oldValue;
				--newValue.counter;

				if (InterlockedCompareExchange64(&_value.as64, newValue.as64, oldValue.as64) != oldValue.as64) {
					continue;
				}
#ifdef _DEBUG_
				if (newValue.counter == 0) __asm int 3;
#endif
				return;
			} else {
				break;
			}
		}

		derefOld(ptr, 1);
	}

	bool updatePointer(T* oldPtr, T* newPtr)
	{
		while (true) {
			Value oldValue = _value;
			if (oldValue.ptr != oldPtr) return false;

			Value newValue;
			newValue.ptr = newPtr;
			newValue.counter = 1;

			if (InterlockedCompareExchange64(&_value.as64, newValue.as64, oldValue.as64) != oldValue.as64) {
				continue;
			}

			if (InterlockedExchangeAdd(&oldPtr->_counter, oldValue.counter - 2) == 2 - oldValue.counter) {
#ifdef _DEBUG_
				InterlockedIncrement(&numDeleted_immediately);
#endif // _DEBUG_
				free(oldPtr);
			}

			break;
		}

		return true;
	}

private:
	union Value {
		struct {
			T*  ptr;
			int counter;
		};

		volatile LONG64 as64;
	};

	Value _value;
};