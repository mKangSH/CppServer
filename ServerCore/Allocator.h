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