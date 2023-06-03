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

// Memory ���� ���� �ذῡ ����
// Use-After-Free(Dangling Pointer)
// Casting 

// Memory Overflow ���� ����
class StompAllocator
{
	enum { PAGE_SIZE = 0x1000 };

public:
	static void*	Alloc(int32 size);
	static void		Release(void* ptr);
};