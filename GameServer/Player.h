#pragma once

class Player
{

public:
	uint64					playerId = 0;
	string					name;
	Protocol::PlayerType	type = Protocol::PLAYER_TYPE_NONE;
	
	GameSessionRef			ownerSession; // 세션에서도 플레이어 알고 플레이어도 세션을 알면 Cycle 발생 !
};

