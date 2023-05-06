#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"
#include "DeadLockProfiler.h"

ThreadManager* GThreadManager = nullptr;
DeadLockProfiler* GDeadLockProfiler = nullptr;

CoreGlobal::CoreGlobal()
{
	GThreadManager = new ThreadManager();
#ifdef _DEBUG
	GDeadLockProfiler = new DeadLockProfiler();
#endif

}

CoreGlobal::~CoreGlobal()
{
	delete GThreadManager;
#ifdef _DEBUG
	delete GDeadLockProfiler;
#endif
}