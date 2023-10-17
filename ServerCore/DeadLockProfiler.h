#pragma once
#include <stack>
#include <map>
#include <vector>

/************************************
*		  DeadLock Profiler			*
*************************************/

// �ʱ� ���� �� ��Ƽ�����尡 �ƴ� TLS ������ �� ������ �����ϰ� ������⿡ ���� �߻�!
// PopLock -> INVALID_UNLOCK
// if ) Lock Stack 1: 0 -> 1 -> 2
//		Lock Stack 2: 3 -> 1 -> 5
// �� �����ؾ� �ϴµ� LockStack�� ���� �޸𸮿� ����ϰ� �־ �ٸ� lock Stack�� �����Ͱ� ������
// ������ ���̸鼭 Error�� �߻�. 
// �ذ� ��� =>		1) DeadLock Profiler�� TLS �������η� ����
//					2) Lock Stack�� TLS �������� ����
class DeadLockProfiler
{
public:
	void PushLock(const char* name);
	void PopLock(const char* name);
	void CheckCycle();

private:
	void Dfs(int32 here);

private:
	unordered_map<const char*, int32>	_nameToId;
	unordered_map<int32, const char*>	_idToName;
	map<int32, set<int32>>				_lockHistory;

	Mutex _lock;

private:
	// ������ �߰ߵ� ���� ����ϴ� �迭
	vector<int32>	_discoveredOrder; 
	// ������ �߰ߵ� ����
	int32			_discoveredCount = 0;
	// Dfs(i) ���� ?
	vector<bool>	_finished;
	vector<int32>	_parent;
};

