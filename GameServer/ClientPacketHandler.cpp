#include "pch.h"
#include "ClientPacketHandler.h"
#include "Player.h"
#include "Room.h"
#include "GameSession.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

// 컨텐츠 작업자가 처리

bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	// TODO : Log

	return false;
}

bool Handle_C_LOGIN(PacketSessionRef& session, Protocol::C_LOGIN& pkt)
{
	GameSessionRef gameSession = static_pointer_cast<GameSession>(session);

	// TODO : Validation Check (Login 패킷이 여러번 오는 경우 등)

	Protocol::S_LOGIN loginPkt;
	loginPkt.set_success(true);

	// DB에서 플레이어 정보 긁어온다.
	// GameSession에 플레이 정보를 저장 (메모리)

	// ID 발급 (DB가 아닌, 인게임 아이디)
	static Atomic<uint64> idGenerator = 1;

	{
		auto player = loginPkt.add_players();
		player->set_name(u8"DB에서긁은이름");
		player->set_playertype(Protocol::PLAYER_TYPE_KNIGHT);

		PlayerRef playerRef = MakeShared<Player>();
		playerRef->playerId = idGenerator++;
		playerRef->name = player->name();
		playerRef->type = player->playertype();
		playerRef->ownerSession = gameSession;

		gameSession->_players.push_back(playerRef);
	}

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(loginPkt);
	session->Send(sendBuffer);

	return true;
}

bool Handle_C_ENTER_GAME(PacketSessionRef& session, Protocol::C_ENTER_GAME& pkt)
{
	GameSessionRef gameSession = static_pointer_cast<GameSession>(session);

	uint64 index = pkt.playerindex();
	// TODO : Validation Check

	PlayerRef player = gameSession->_players[index]; // Read_only
	GRoom.PushJob(MakeShared<EnterJob>(GRoom, player));


	// TODO Enter 이후에 실행되는 아래 구문을 적절한 위치로 옮겨야 함
	Protocol::S_ENTER_GAME enterGamePkt;
	enterGamePkt.set_success(true);

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(enterGamePkt);
	player->ownerSession->Send(sendBuffer);

	return true;
}

bool Handle_C_CHAT(PacketSessionRef& session, Protocol::C_CHAT& pkt)
{
	std::cout << pkt.msg() << endl;

	Protocol::S_CHAT chatPkt;
	chatPkt.set_msg(pkt.msg());

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(chatPkt);
	GRoom.PushJob(MakeShared<BroadcastJob>(GRoom, sendBuffer)); // WRITE_LOCK

	return true;
}
