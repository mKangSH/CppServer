#pragma once

extern class ThreadManager* GThreadManager;

// 매니저가 많아질 경우
// 매니저의 생성 및 삭제 순서를 맞추기 위한 클래스
class CoreGlobal
{
public:
	CoreGlobal();
	~CoreGlobal();
};

