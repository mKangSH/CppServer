#include "pch.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "ClientPacketHandler.h"
#include "Room.h"

void GameSession::OnConnected()
{
	GSessionManager.Add(static_pointer_cast<GameSession>(shared_from_this()));
}

void GameSession::OnDisconnected()
{
	GSessionManager.Remove(static_pointer_cast<GameSession>(shared_from_this()));
	
	if (_currentPlayer)
	{
		if (auto room = _room.lock())
		{
			room->DoAsync(&Room::Leave, _currentPlayer);
		}
	}

	_currentPlayer = nullptr;
	_players.clear();
}

void GameSession::OnRecvPacket(BYTE* buffer, int32 len)
{
	PacketSessionRef session = GetPacketSessionRef();
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);

	// TODO : PacketId 대역 체크
	// 서버가 여러 종류로 붙을 경우
	// 패킷 헤더를 참고하여 아이디 대역대, 사이즈 등을
	// 참고하여 클라이언트, DB 서버, 게임 서버 등
	// 받아온 패킷이 어디서 보내온 데이터인지 확인해야 함

	ClientPacketHandler::HandlePacket(session, buffer, len);
}

void GameSession::OnSend(int32 len)
{
	
}