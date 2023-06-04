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

CoreGlobal GCoreGlobal;

class Player
{
public:
	Player() {}
	virtual ~Player() {}
};

class Knight : public Player
{
public:
	Knight()
	{
		cout << "Knight()" << endl;
	}

	Knight(int32 hp) : _hp(hp)
	{
		cout << "Knight(hp)" << endl;
	}

	~Knight()
	{
		cout << "~Knight()" << endl;
	}

	int _hp = 100;
	int _mp = 10;
};

int main()
{
	int32 knightSize = sizeof(Knight);

	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch([]()
			{
				while (true)
				{
					xVector<Knight> v(10);

					xMap<int32, Knight> m;
					m[100] = Knight();

					this_thread::sleep_for(10ms);
				}
			});
	}

	GThreadManager->Join();
}