#pragma once

class GameSession;
using GameSessionRef = shared_ptr<GameSession>;

// Service Class�� �����ѵ� ���� �ھ �ƴ�
// ������ �ܿ��� ���ϰ� ����ϱ� ���� ���� Ŭ����
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