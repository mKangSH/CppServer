#pragma once

extern class ThreadManager*		GThreadManager;
extern class MemoryManager*		GMemoryManager;
extern class SendBufferManager* GSendBufferManager;
extern class GlobalQueue*		GGlobalQueue;
extern class JobTimer* GJobTimer;

#ifdef _DEBUG
extern class DeadLockProfiler*	GDeadLockProfiler;
#endif

// �Ŵ����� ������ ���
// �Ŵ����� ���� �� ���� ������ ���߱� ���� Ŭ����