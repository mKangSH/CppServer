﻿#include "pch.h"
#include "ThreadManager.h"
#include "Service.h"
#include "Session.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "BufferWriter.h"
#include "ClientPacketHandler.h"
#include <tchar.h>
#include "Protocol.pb.h"
#include "Room.h"
#include "Job.h"

int main()
{
	// TEST JOB
	{
		// [의뢰 내용] : 1번 유저에게 10만큼 힐을 줘라
		// [행동] : Heal
		// [인자] : 1번 유저, 힐량 10
		HealJob healJob;
		healJob._target = 1;
		healJob._healValue = 10;

		// 나중에
		healJob.Execute();
	}

	// JOB
	{

	}

	ClientPacketHandler::Init();

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
		GRoom.FlushJob();
		this_thread::sleep_for(1ms);
	}

	GThreadManager->Join();
}