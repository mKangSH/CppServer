#pragma once
#include "Allocator.h"

// Memory pooling ���� ����� �����ϱ� ���� �������̽�
class MemoryPool;

/************************************
*			   Memory   			*
*************************************/
class MemoryManager
{
	enum
	{
		// ~1024 Bytes : 32Bytes ���� Pool
		// ~2048 Bytes : 128Bytes ���� Pool
		// ~4096 Bytes : 256Bytes ���� Pool
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

	// �޸� ũ�� <=> �޸� Ǯ
	// O(1) ������ ã�� ���� ���̺�
	MemoryPool* _poolTable[MAX_ALLOC_SIZE + 1];
};


// typename... : variadic template
template<typename Type, typename... Args>
Type* xnew(Args&&... args)
{
	Type* memory = static_cast<Type*>(PoolAllocator::Alloc(sizeof(Type)));

	// placement new
	// �޸� ���� ��ü �����ڸ� �ø��� ����
	// new(memory)Type();

	new(memory)Type(std::forward<Args>(args)...);
	return memory;
}

template<typename Type>
void xdelete(Type* obj)
{
	// �Ҹ��� ȣ��
	obj->~Type();

	// �޸� �ݳ�
	PoolAllocator::Release(obj);
}

template<typename Type>
shared_ptr<Type> MakeShared()
{
	return shared_ptr<Type>( xnew<Type>(), xdelete<Type>);
}