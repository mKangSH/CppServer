#include "pch.h"
#include "Lock.h"

void Lock::WriteLock()
{
	// 동일한 thread가 lock을 소유하고 있다면 무조건 성공 (재귀 호출 대비)
	const uint32 lockThreadId = (_lockFlag.load() & WRITE_THREAD_MASK) >> 16;
	if (LThreadId == lockThreadId)
	{
		_writeCount++;
		return;
	}

	// 아무도 소유(상호배타적) 및 공유(ReadLock) x => 경합해서 소유권 획득
	const uint64 beginTick = GetTickCount64();
	const uint32 desired = ((LThreadId << 16) & WRITE_THREAD_MASK);

	while (true)
	{
		for (uint32 spinCount = 0; spinCount < MAX_SPIN_COUNT; spinCount++)
		{
			uint32 expected = EMPTY_FLAG;
			if (_lockFlag.compare_exchange_strong(OUT expected, desired))
			{
				// 재귀적 호출 시에 Crash 대신
				// WriteLocK 한 번 더 실행하고 _writeCount로 추적
				_writeCount++;
				return;
			}
		}

		// DEAD LOCK 등 컨텐츠 문제 발생했을 가능성 농후
		if (GetTickCount64() - beginTick >= ACQUIRE_TIMEOUT_TICK)
		{
			CRASH("LOCK_TIMEOUT");
		}

		this_thread::yield();
	}
}

void Lock::WriteUnlock()
{
	// ReadLock 을 모두 풀기 전에 WriteUnlock 불가
	if ((_lockFlag.load() & READ_COUNT_MASK) != 0)
	{
		CRASH("INVALID_UNLOCK_ORDER");
	}

	const int32 lockCount = --_writeCount;
	if (lockCount == 0)
	{
		// _lockFlag.store(_lockFlag.load() & !(WRITE_THREAD_MASK));
		_lockFlag.store(EMPTY_FLAG);
	}
}

void Lock::ReadLock()
{
	// 동일한 thread가 lock을 소유하고 있다면 무조건 성공 (재귀 호출 대비)
	const uint32 lockThreadId = (_lockFlag.load() & WRITE_THREAD_MASK) >> 16;
	if (LThreadId == lockThreadId)
	{
		_lockFlag.fetch_add(1);
		return;
	}

	// 아무도 소유하고 있지 않을 때, 경합해서 공유 카운트 증가
	const uint64 beginTick = GetTickCount64();

	while (true)
	{
		for (uint32 spinCount = 0; spinCount < MAX_SPIN_COUNT; spinCount++)
		{
			uint32 expected = (_lockFlag.load() & READ_COUNT_MASK);
			if (_lockFlag.compare_exchange_strong(OUT expected, expected + 1))
			{
				return;
			}
		}

		if (GetTickCount64() - beginTick >= ACQUIRE_TIMEOUT_TICK)
		{
			CRASH("LOCK_TIMEOUT");
		}
		this_thread::yield();
	}
}

void Lock::ReadUnlock()
{
	if ((_lockFlag.fetch_sub(1) & READ_COUNT_MASK) == 0)
	{
		CRASH("MULTIPLE_UNLOCK");
	}
}
