#pragma once
#include <stack>
#include <map>
#include <vector>

/************************************
*		  DeadLock Profiler			*
*************************************/

// 초기 설계 시 멀티쓰레드가 아닌 TLS 영역에 들어갈 것으로 예상하고 만들었기에 문제 발생!
// PopLock -> INVALID_UNLOCK
// if ) Lock Stack 1: 0 -> 1 -> 2
//		Lock Stack 2: 3 -> 1 -> 5
// 로 동작해야 하는데 LockStack을 공용 메모리에 사용하고 있어서 다른 lock Stack의 데이터가 끼어들어
// 순서가 꼬이면서 Error가 발생. 
// 해결 방안 =>		1) DeadLock Profiler를 TLS 영역으로로 수정
//					2) Lock Stack을 TLS 영역으로 수정
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
	// 정점이 발견된 순서 기록하는 배열
	vector<int32>	_discoveredOrder; 
	// 정점이 발견된 순서
	int32			_discoveredCount = 0;
	// Dfs(i) 종료 ?
	vector<bool>	_finished;
	vector<int32>	_parent;
};

