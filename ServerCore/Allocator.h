#pragma once

/************************************
*		   Base Allocator			*
*************************************/
class BaseAllocator
{
public:
	static void*	Alloc(int32 size);
	static void		Release(void* ptr);
};


/************************************
*		   Stomp Allocator			*
*************************************/

// Memory 오염 문제 해결에 유용
// Use-After-Free(Dangling Pointer)
// Casting 

// Memory Overflow 감지 못함
class StompAllocator
{
	enum { PAGE_SIZE = 0x1000 };

public:
	static void*	Alloc(int32 size);
	static void		Release(void* ptr);
};

/************************************
*		    STL Allocator			*
*************************************/

template <typename T>
class StlAllocator
{
public:
	using value_type = T;

	StlAllocator() {}

	template<typename Other>
	StlAllocator(const StlAllocator<Other>&) {}

	T* allocate(size_t count)
	{
		const int32 size = static_cast<int32>(count * sizeof(T));
		return static_cast<T*>(xAlloc(size));
	}

	void deallocate(T* ptr, size_t count)
	{
		xRelease(ptr);
	}
};