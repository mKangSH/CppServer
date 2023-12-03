#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"
#include "Memory.h"
#include "DeadLockProfiler.h"
#include "SocketUtils.h"
#include "SendBuffer.h"
#include "GlobalQueue.h"

ThreadManager*		GThreadManager = nullptr;
MemoryManager*		GMemoryManager = nullptr;
SendBufferManager*	GSendBufferManager = nullptr;
GlobalQueue*		GGlobalQueue = nullptr;

#ifdef _DEBUG
DeadLockProfiler*	GDeadLockProfiler = nullptr;
#endif

class CoreGlobal
{
public:
	CoreGlobal()
	{
		GThreadManager = new ThreadManager();
		GMemoryManager = new MemoryManager();
		GSendBufferManager = new SendBufferManager();
		GGlobalQueue = new GlobalQueue();
		SocketUtils::Init();

#ifdef _DEBUG
		GDeadLockProfiler = new DeadLockProfiler();
#endif

	}

	~CoreGlobal()
	{
		delete GThreadManager;
		delete GMemoryManager;
		delete GSendBufferManager;
		delete GGlobalQueue;
		SocketUtils::Clear();

#ifdef _DEBUG
		delete GDeadLockProfiler;
#endif
	}
}GCoreGlobal;