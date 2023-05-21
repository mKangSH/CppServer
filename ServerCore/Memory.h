#pragma once
#include "Allocator.h"

// typename... : variadic template
template<typename Type, typename... Args>
Type* xnew(Args&&... args)
{
	Type* memory = static_cast<Type*>(BaseAllocator::Alloc(sizeof(Type)));

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
	BaseAllocator::Release(obj);
}