#pragma once

extern class ThreadManager* GThreadManager;
extern class MemoryManager* GMemoryManager;

#ifdef _DEBUG
extern class DeadLockProfiler* GDeadLockProfiler;
#endif

// �Ŵ����� ������ ���
// �Ŵ����� ���� �� ���� ������ ���߱� ���� Ŭ����
class CoreGlobal
{
public:
	CoreGlobal();
	~CoreGlobal();
};