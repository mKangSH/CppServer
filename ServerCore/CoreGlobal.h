#pragma once

extern class ThreadManager*		GThreadManager;
extern class MemoryManager*		GMemoryManager;
extern class SendBufferManager* GSendBufferManager;

#ifdef _DEBUG
extern class DeadLockProfiler*	GDeadLockProfiler;
#endif

// �Ŵ����� ������ ���
// �Ŵ����� ���� �� ���� ������ ���߱� ���� Ŭ����