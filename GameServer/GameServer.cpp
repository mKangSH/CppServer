#include "pch.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <Windows.h>
#include <future>

#include "ThreadManager.h"
#include "Allocator.h"
#include "Memory.h"
#include "LockFreeStack.h"

// CoreGlobal 객체 생성
CoreGlobal GCoreGlobal;

// 실질적인 코드 작성부
DECLSPEC_ALIGN(16)
class Data // : public SListEntry
{
public:
	SListEntry _entry;

	int64 _rand = rand() % 1000;
};

SListHeader* GHeader;

int main()
{
	GHeader = new SListHeader();
	ASSERT_CRASH(((uint64)GHeader % 16) == 0);
	InitializeHead(GHeader);

	for (int32 i = 0; i < 3; i++)
	{
		GThreadManager->Launch([]()
			{
				while (true)
				{
					Data* data = new Data();

					// Align malloc 하는 방법도 있음
					ASSERT_CRASH(((uint64)GHeader % 16) == 0);

					PushEntrySList(GHeader, (SListEntry*)data);
					this_thread::sleep_for(10ms);
				}
			});
	}

	for (int32 i = 0; i < 2; i++)
	{
		GThreadManager->Launch([]()
			{
				while (true)
				{
					Data* pop = nullptr;
					pop = (Data*)PopEntrySList(GHeader);

					if (pop)
					{
						cout << pop->_rand << endl;
						delete pop;
					}

					else
					{
						cout << "NONE" << endl;
					}
				}
			});
	}

	GThreadManager->Join();
}