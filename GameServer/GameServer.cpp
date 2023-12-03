#include "pch.h"
#include "ThreadManager.h"
#include "Service.h"
#include "Session.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "BufferWriter.h"
#include "ClientPacketHandler.h"
#include <tchar.h>
#include <functional>
#include "Protocol.pb.h"
#include "Room.h"
#include "Job.h"
#include "Player.h"

enum
{
	WORKER_TICK = 64,
};

void DoWorkerJob(ServerServiceRef& service)
{
	while (true)
	{
		// WORKER Tick 하드코딩된 부분을 바꾸는 법도 생각
		LEndTickCount = ::GetTickCount64() + WORKER_TICK;
		
		// Network IO 처리 -> 인게임 로직까지 (Packet Handler에 의해)
		service->GetIocpCore()->Dispatch(10);

		// Global Queue
		ThreadManager::DoGlobalQueueWork();
	}
}

int main()
{
	ClientPacketHandler::Init();

	ServerServiceRef service = MakeShared<ServerService>(
		NetAddress(L"127.0.0.1", 7777), 
		MakeShared<IocpCore>(), 
		MakeShared<GameSession>, // TODO : Sessionmanager 등
		100);

	ASSERT_CRASH(service->Start());

	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch([&service]()
			{
				while (true)
				{
					DoWorkerJob(service);
				}
			});
	}

	// Main Thread
	DoWorkerJob(service);

	GThreadManager->Join();
}