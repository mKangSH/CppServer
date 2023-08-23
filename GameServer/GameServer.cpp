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
typedef enum PlayerType
{
	KNIGHT,
	MAGE,
	ARCHER,

}PlayerType;


using TL = TypeList<class Player, class Mage, class Knight, class Archer>;

class Player
{
public:
	Player()
	{
		INIT_TL(Player)
	}

	virtual ~Player() {}

	DECLARE_TL
};

class Knight : public Player
{
public:
	Knight() { INIT_TL(Knight) }
};

class Mage : public Player
{
public:
	Mage() { INIT_TL(Mage) }
};

class Archer : public Player
{
public:
	Archer() { INIT_TL(Archer) }
};

class Monster
{
public:
	int64 _id = 0;
};

int main()
{
	/*TypeList<Mage, Knight>::Head whoAmI;
	TypeList<Mage, Knight>::Tail whoAmI2;

	TypeList<Mage, TypeList<Knight, Archer>>::Head whoAmI3;
	TypeList<Mage, TypeList<Knight, Archer>>::Tail::Head whoAmI4;
	TypeList<Mage, TypeList<Knight, Archer>>::Tail::Tail whoAmI5;

	int32 len1 = Length<TypeList<Mage, Knight>>::value;
	int32 len2 = Length<TypeList<Mage, Knight, Archer>>::value;
	int32 len3 = Length<TypeList<>>::value;

	TypeAt<TL, 0>::Result WhoAmI6;
	TypeAt<TL, 1>::Result WhoAmI7;
	TypeAt<TL, 2>::Result WhoAmI8;

	int32 index1 = IndexOf<TL, Mage>::value;
	int32 index2 = IndexOf<TL, Knight>::value;
	int32 index3 = IndexOf<TL, Archer>::value;
	int32 index4 = IndexOf<TL, Monster>::value;

	bool canConvert1 = Conversion<Player, Knight>::exists;
	bool canConvert2 = Conversion<Knight, Player>::exists;
	bool canConvert3 = Conversion<Knight, Monster>::exists;
	
	TypeConversion<TL> test;*/

	//{
	//	Player* player = new Knight();

	//	bool canCast = CanCast<Knight*>(player);
	//	Knight* knight = TypeCast<Knight*>(player);
	//	
	//	if (knight)
	//	{
	//		// ~~
	//	}

	//	delete player;
	//}

	{
		shared_ptr<Player> player = MakeShared<Knight>();

		shared_ptr<Archer> archer = TypeCast<Archer>(player);
		bool canCast = CanCast<Mage>(player);
	}

	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch([]()
			{
				while (true)
				{
				}
			});
	}

	GThreadManager->Join();
}