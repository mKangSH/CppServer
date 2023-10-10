#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"
#include "Memory.h"
#include "DeadLockProfiler.h"
#include "SocketUtils.h"

ThreadManager* GThreadManager = nullptr;
MemoryManager* GMemoryManager = nullptr;

#ifdef _DEBUG
DeadLockProfiler* GDeadLockProfiler = nullptr;
#endif
class CoreGlobal
{
public:
	CoreGlobal()
	{
		GThreadManager = new ThreadManager();
		GMemoryManager = new MemoryManager();
		SocketUtils::Init();

#ifdef _DEBUG
		GDeadLockProfiler = new DeadLockProfiler();
#endif

	}

	~CoreGlobal()
	{
		delete GThreadManager;
		delete GMemoryManager;
		SocketUtils::Clear();

#ifdef _DEBUG
		delete GDeadLockProfiler;
#endif
	}
}GCoreGlobal;