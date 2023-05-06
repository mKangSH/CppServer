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
	void WriteLock(const char* name);
	void WriteUnlock(const char* name);
	void ReadLock(const char* name);
	void ReadUnlock(const char* name);

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
	ReadLockGuard(Lock& lock, const char* name) : _lock(lock), _name(name) { _lock.ReadLock(name); }
	~ReadLockGuard() { _lock.ReadUnlock(_name); }

private:
	Lock& _lock;
	const char* _name;
};

class WriteLockGuard
{
public:
	WriteLockGuard(Lock& lock, const char* name) : _lock(lock), _name(name) { _lock.WriteLock(name); }
	~WriteLockGuard() { _lock.WriteUnlock(_name); }

private:
	Lock& _lock;
	const char* _name;
};