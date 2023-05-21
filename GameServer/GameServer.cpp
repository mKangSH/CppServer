#include "pch.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <Windows.h>
#include <future>

#include "RefCounting.h"

class Wraith
{
public:
	Wraith()
	{

	}

	~Wraith()
	{

	}

	static void* operator new(size_t size)
	{
		// new delete 타이밍 intercept 가능
		cout << "Wraith new 실행! 크기: " << size << "bytes." << endl;
		void* ptr = ::malloc(size);
		return ptr;
	}

	static void operator delete(void* ptr)
	{
		cout << "Wraith delete 실행!" << endl;
		::free(ptr);
	}


	int _hp = 150;
	int _posX = 0;
	int _posY = 0;
	int _armor = 1;
};

// new operator overloading (Global)

//void* operator new(size_t size)
//{
//	// new delete 타이밍 intercept 가능
//	cout << "new 실행! 크기: " << size << "bytes." << endl;
//	void* ptr = ::malloc(size);
//	return ptr;
//}
//
//void operator delete(void* ptr)
//{
//	cout << "Delete 실행!" << endl;
//	::free(ptr);
//}
//
//void* operator new[](size_t size)
//{
//	cout << "new[] 실행! 크기: " << size << "bytes." << endl;
//	void* ptr = ::malloc(size);
//	return ptr;
//}
//
//void operator delete[](void* ptr)
//{
//	cout << "delete[] 실행!" << endl;
//	::free(ptr);
//}

int main()
{
	// shared_ptr<Wraith> spr = make_shared<Wraith>();
	// weak_ptr<Wraith> wpr = spr;

	// bool expired = wpr.expired(); // 존재?
	
	// if expired == true
	// spr = wpr;

	// shared_ptr<Wraith> spr2 = wpr.lock();
	// if spr2 != nullptr
	// {}

	Wraith* w1 = new Wraith();

	delete w1;
}