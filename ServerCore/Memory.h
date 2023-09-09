#pragma once
#include "Allocator.h"

// Memory pooling 등의 기법을 적용하기 위한 인터페이스
class MemoryPool;

/************************************
*			   Memory   			*
*************************************/
class MemoryManager
{
	enum
	{
		// ~1024 Bytes : 32Bytes 단위 Pool
		// ~2048 Bytes : 128Bytes 단위 Pool
		// ~4096 Bytes : 256Bytes 단위 Pool
		POOL_COUNT = (1024 / 32) + (1024 / 128) + (2048 / 256),
		MAX_ALLOC_SIZE = 4096
	};

public:
	MemoryManager();
	~MemoryManager();

	void*	Allocate(int32 size);
	void	Release(void* ptr);

private:
	vector<MemoryPool*> _pools;

	// 메모리 크기 <=> 메모리 풀
	// O(1) 빠르게 찾기 위한 테이블
	MemoryPool* _poolTable[MAX_ALLOC_SIZE + 1];
};


// typename... : variadic template
template<typename Type, typename... Args>
Type* xnew(Args&&... args)
{
	Type* memory = static_cast<Type*>(PoolAllocator::Alloc(sizeof(Type)));

	// placement new
	// 메모리 위에 객체 생성자를 올리는 문법
	// new(memory)Type();

	new(memory)Type(std::forward<Args>(args)...);
	return memory;
}

template<typename Type>
void xdelete(Type* obj)
{
	// 소멸자 호출
	obj->~Type();

	// 메모리 반납
	PoolAllocator::Release(obj);
}

template<typename Type>
shared_ptr<Type> MakeShared()
{
	return shared_ptr<Type>( xnew<Type>(), xdelete<Type>);
}