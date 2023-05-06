#include "pch.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <Windows.h>
#include <future>
#include "ThreadManager.h"

#include "AccountManager.h"
#include "PlayerManager.h"

CoreGlobal coreGlobal;

int main()
{
	GThreadManager->Launch([=]
		{
			while (true)
			{
				cout << "PlayerAccount\n";
				GPlayerManager.PlayerThenAccount();
				this_thread::sleep_for(10ms);
			}
		});

	GThreadManager->Launch([=]
		{
			while (true)
			{
				cout << "AccountThenPlayer\n";
				GAccountManager.AccountThenPlayer();
				this_thread::sleep_for(10ms);
			}
		});

	GThreadManager->Join();
}