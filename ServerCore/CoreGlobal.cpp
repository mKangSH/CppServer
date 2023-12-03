#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"
#include "Memory.h"
#include "DeadLockProfiler.h"
#include "SocketUtils.h"
#include "SendBuffer.h"
#include "GlobalQueue.h"
#include "JobTimer.h"

ThreadManager*		GThreadManager = nullptr;
MemoryManager*		GMemoryManager = nullptr;
SendBufferManager*	GSendBufferManager = nullptr;
GlobalQueue*		GGlobalQueue = nullptr;
JobTimer*			GJobTimer = nullptr;

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
		GJobTimer = new JobTimer();
		
#ifdef _DEBUG
		GDeadLockProfiler = new DeadLockProfiler();
#endif

		SocketUtils::Init();
	}

	~CoreGlobal()
	{
		delete GThreadManager;
		delete GMemoryManager;
		delete GSendBufferManager;
		delete GGlobalQueue;
		delete GJobTimer;

#ifdef _DEBUG
		delete GDeadLockProfiler;
#endif

		SocketUtils::Clear();
	}
}GCoreGlobal;