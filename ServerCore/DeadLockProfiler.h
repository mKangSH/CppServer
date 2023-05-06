#pragma once
#include <stack>
#include <map>
#include <vector>

/************************************
*		  DeadLock Profiler			*
*************************************/

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
	stack<int32>						_lockStack;
	map<int32, set<int32>>				_lockHistory;

	Mutex _lock;

private:
	// 정점이 발견된 순서 기록하는 배열
	vector<int32>	_discoveredOrder; 
	// 정점이 발견된 순서
	int32			_discoveredCount = 0;
	// Dfs(i) 종료 ?
	vector<bool>	_finished;
	vector<int32>	_parent;
};

