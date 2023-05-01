#include "pch.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <Windows.h>

mutex m;
queue<int32> q;
HANDLE handle = NULL;

// User-Level Object(표준 mutex와 짝지어 동작)
condition_variable cv;

void Producer()
{
	// 1) lock
	// 2) 공유 변수 값 수정
	// 3) unlock
	// 4) 조건 변수를 통해 다른 쓰레드에 통지
	while (true)
	{
		{
			unique_lock<mutex> lock(m);
			q.push(100);
		}

		// wait 중인 쓰레드 1개를 깨운다.
		cv.notify_one(); 
	}
}

void Consumer()
{
	while (true)
	{
		unique_lock<mutex> lock(m);
		// 1) lock
		// 2) 조건 확인
		//  - true => 이어서 코드 진행
		//  - false => unlock 후 대기 상태
		cv.wait(lock, []() { return q.empty() == false; });

		if (q.empty() == false)
		{
			int32 data = q.front();
			q.pop();

			cout << data << '\n';
		}
	}
}


int main()
{
	// Kernel Object
	// Usage Count
	// Signal / Non-Signal << bool
	// Auto / Manual << bool
	handle = ::CreateEvent(/*보안속성*/NULL, /*bManualReset*/ FALSE, /*bInitialState*/FALSE, /*이름 필수x*/NULL);

	if (handle == NULL)
	{
		return 0;
	}
	
	thread t1(Producer);
	thread t2(Consumer);

	t1.join();
	t2.join();

	::CloseHandle(handle);
}