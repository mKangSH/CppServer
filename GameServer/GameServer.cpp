#include "pch.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <Windows.h>
#include <future>
#include "ThreadManager.h"

#include <WinSock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>
// 매우 중요
#pragma comment(lib, "ws2_32.lib")

#include "Memory.h"

CoreGlobal GCoreGlobal;

int main()
{
	
}