#pragma once
#include "Allocator.h"

// typename... : variadic template
template<typename Type, typename... Args>
Type* xnew(Args&&... args)
{
	Type* memory = static_cast<Type*>(BaseAllocator::Alloc(sizeof(Type)));

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
	BaseAllocator::Release(obj);
}