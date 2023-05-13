#include "pch.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <Windows.h>
#include <future>

#include "RefCounting.h"

class Wraith : public RefCountable
{
public:
	int _hp = 150;
	int _posX = 0;
	int _posY = 0;
	int _armor = 1;
};

using WraithRef = TSharedPtr<Wraith>;

class Missile : public RefCountable
{
public:
	void SetTarget(WraithRef target)
	{
		_target = target;

		// reference count: 3 확인용 임시 중단 변수
		int i = 32;
	}

	bool Update()
	{
		if (_target == nullptr)
		{
			return true;
		}

		int posX = _target->_posX;
		int posY = _target->_posY;

		if (_target->_hp == 0)
		{
			_target->ReleaseRef();
			_target = nullptr;
			return true;
		}

		return false;
	}

private:
	WraithRef _target = nullptr;
};

using MissileRef = TSharedPtr<Missile>;

int main()
{
	WraithRef wraith(new Wraith());
	MissileRef missile(new Missile());

 	missile->SetTarget(wraith);
	wraith->_hp = 0;

	if(wraith->_hp == 0)
	{
		// wraith = WraithRef(nullptr);
		wraith = nullptr;
	}

	while (true)
	{
		if (missile)
		{
			if (missile->Update())
			{
				missile->ReleaseRef();
				missile = nullptr;
			}
		}
	}
}