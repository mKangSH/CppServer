﻿#include "pch.h"
#include "ThreadManager.h"
#include "Service.h"
#include "Session.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "BufferWriter.h"
#include "ServerPacketHandler.h"
#include <tchar.h>

// 패킷 설계 TEMP
#pragma pack(1)
struct PKT_S_TEST
{
	uint64 id; // 8
	uint32 hp; // 4
	uint16 attack; // 2

	// 가변 데이터
	// 1) 문자열 (ex.name) Encoding Issue
	// 2) 일반 바이트 배열 (ex. 길드 이미지)
	// 3) 일반 리스트

	// vector<BuffData> buffs;
	// wstring name;
};
#pragma pack()

int main()
{
	PKT_S_TEST pkt;

	ServerServiceRef service = MakeShared<ServerService>(
		NetAddress(L"127.0.0.1", 7777), 
		MakeShared<IocpCore>(), 
		MakeShared<GameSession>, // TODO : Sessionmanager 등
		100);

	ASSERT_CRASH(service->Start());

	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch([=]()
			{
				while (true)
				{
					service->GetIocpCore()->Dispatch();
				}
			});
	}

	while (true)
	{
		vector<BuffData> buffs{ BuffData{100, 1.5f}, BuffData{200, 2.3f}, BuffData {300, 0.7f} };
		SendBufferRef sendBuffer = ServerPacketHandler::Make_S_TEST(1001, 100, 10, buffs, L"하");
		GSessionManager.BroadCast(sendBuffer);

		this_thread::sleep_for(250ms);
	}

	GThreadManager->Join();
}