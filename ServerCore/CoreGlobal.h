#pragma once

extern class ThreadManager*		GThreadManager;
extern class MemoryManager*		GMemoryManager;
extern class SendBufferManager* GSendBufferManager;
extern class GlobalQueue*		GGlobalQueue;
extern class JobTimer* GJobTimer;

#ifdef _DEBUG
extern class DeadLockProfiler*	GDeadLockProfiler;
#endif

// 매니저가 많아질 경우
// 매니저의 생성 및 삭제 순서를 맞추기 위한 클래스