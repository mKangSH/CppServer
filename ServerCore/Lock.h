#pragma once
#include "Types.h"

/************************************
*		   W/R SpinLock				*
*************************************/

// 상위 16bits Write flag (Exclusive Lock Owner ThreadID)
// 하위 16bits Read Flag (Shared Read Count)

// Write Lock 잡힌 상태에서 동일 쓰레드가 Read Lock 시도(O)
// Read Lock 잡힌 상태에서 동일 쓰레드가 Write Lock 시도(X)
// W -> W, R -> R (O)

class Lock
{
	enum : uint32
	{
		ACQUIRE_TIMEOUT_TICK = 10000,
		MAX_SPIN_COUNT = 5000,
		WRITE_THREAD_MASK = 0xFFFF'0000,
		READ_COUNT_MASK = 0x0000'FFFF,
		EMPTY_FLAG = 0x0000'0000,
	};

public:
	void WriteLock();
	void WriteUnlock();
	void ReadLock();
	void ReadUnlock();

private:
	Atomic<uint32> _lockFlag = EMPTY_FLAG;
	uint32 _writeCount = 0;
};

/************************************
*		    Lock Guards				*
*************************************/

class ReadLockGuard
{
public:
	ReadLockGuard(Lock& lock) : _lock(lock) { _lock.ReadLock(); }
	~ReadLockGuard() { _lock.ReadUnlock(); }

private:
	Lock& _lock;
};

class WriteLockGuard
{
public:
	WriteLockGuard(Lock& lock) : _lock(lock) { _lock.WriteLock(); }
	~WriteLockGuard() { _lock.WriteUnlock(); }

private:
	Lock& _lock;
};