#pragma once

extern class ThreadManager*		GThreadManager;
extern class MemoryManager*		GMemoryManager;
extern class SendBufferManager* GSendBufferManager;
extern class GlobalQueue*		GGlobalQueue;

#ifdef _DEBUG
extern class DeadLockProfiler*	GDeadLockProfiler;
#endif

// �Ŵ����� ������ ���
// �Ŵ����� ���� �� ���� ������ ���߱� ���� Ŭ����