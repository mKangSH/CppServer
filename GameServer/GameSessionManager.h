#pragma once

class GameSession;
using GameSessionRef = shared_ptr<GameSession>;

// Service Class와 동일한데 서버 코어가 아닌
// 컨텐츠 단에서 편하게 사용하기 위한 관리 클래스
class GameSessionManager
{
public:
	void Add(GameSessionRef session);
	void Remove(GameSessionRef session);
	void BroadCast(SendBufferRef sendBuffer);

private:
	USE_LOCK;
	Set<GameSessionRef> _sessions;

};

extern GameSessionManager GSessionManager;