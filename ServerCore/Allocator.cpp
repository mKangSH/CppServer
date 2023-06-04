#include "pch.h"
#include "Allocator.h"
#include "Memory.h"

/************************************
*		   Base Allocator			*
*************************************/
void* BaseAllocator::Alloc(int32 size)
{
	return ::malloc(size);
}

void BaseAllocator::Release(void* ptr)
{
	::free(ptr);
}


/************************************
*		   Stomp Allocator			*
*************************************/

void* StompAllocator::Alloc(int32 size)
{
	// Memory Overflow 잡는 방법
	const int64 pageCount = (size + PAGE_SIZE - 1) / PAGE_SIZE;
	const int64 dataOffset = pageCount * PAGE_SIZE - size;
	// OS 직접 메모리 요청
	void* baseAddress = ::VirtualAlloc(NULL, pageCount * PAGE_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	// 1byte 단위로 메모리 offset 설정을 하기위해 int8*로 Cast
	return static_cast<void*>(static_cast<int8*>(baseAddress) + dataOffset);
}

void StompAllocator::Release(void* ptr)
{
	const int64 address = reinterpret_cast<int64>(ptr);
	const int64 baseAddress = address - (address % PAGE_SIZE);
	::VirtualFree(reinterpret_cast<void*>(baseAddress), 0, MEM_RELEASE);
}


/************************************
*		   Pool Allocator			*
*************************************/

void* PoolAllocator::Alloc(int32 size)
{
	return GMemoryManager->Allocate(size);
}

void PoolAllocator::Release(void* ptr)
{
	GMemoryManager->Release(ptr);
}
