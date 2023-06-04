#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"
#include "Memory.h"
#include "DeadLockProfiler.h"

ThreadManager* GThreadManager = nullptr;
MemoryManager* GMemoryManager = nullptr;

#ifdef _DEBUG
DeadLockProfiler* GDeadLockProfiler = nullptr;
#endif

CoreGlobal::CoreGlobal()
{
	GThreadManager = new ThreadManager();
	GMemoryManager = new MemoryManager();

#ifdef _DEBUG
	GDeadLockProfiler = new DeadLockProfiler();
#endif

}

CoreGlobal::~CoreGlobal()
{
	delete GThreadManager;
	delete GMemoryManager;

#ifdef _DEBUG
	delete GDeadLockProfiler;
#endif
}