#include "pch.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <Windows.h>
#include <future>
#include "ThreadManager.h"

#include "SocketUtils.h"
CoreGlobal GCoreGlobal;

int main()
{
	SOCKET socket = SocketUtils::CreateSocket();

	SocketUtils::BindAnyAddress(socket, 7777);

	SocketUtils::Listen(socket);

	SOCKET clientSocket = ::accept(socket, nullptr, nullptr);

	cout << "Client Connected!" << endl;

	while (true)
	{

	}
	
	GThreadManager->Join();
}