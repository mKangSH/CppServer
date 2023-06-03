#include "pch.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <Windows.h>
#include <future>

#include "RefCounting.h"
#include "Memory.h"
#include "ThreadManager.h"

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
	Knight* knight = (Knight*)xnew<Player>();

	// Memory Overflow 강제 발생
	knight->_hp = 100;

	xdelete(knight);
}