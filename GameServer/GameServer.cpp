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

// CoreGlobal 객체 생성
CoreGlobal GCoreGlobal;

// 실질적인 코드 작성부
void HandleError(const char* cause)
{
	int32 errCode = ::WSAGetLastError();
	cout << cause << "ErrorCode : " << errCode << endl;
	::WSACleanup();
}

int main()
{
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		return -1;
	}

	// 블로킹(Blocking) 소켓, TCP 기준
	// accept -> 접속한 클라가 있을 때
	// connect -> 서버 접속 성공했을 때
	// send, sendto -> 요청한 데이터를 송신 버퍼에 복사했을 때
	// recv, recvfrom -> 수신 버퍼에 도착한 데이터가 있고, 이를 유저레벨 버퍼에 복사했을 때

	// 논블로킹(Non-Blocking) 소켓
	SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET)
	{
		return -1;
	}

	// non-blocking 세팅
	u_long on = 1;
	if (::ioctlsocket(listenSocket, FIONBIO, &on) == INVALID_SOCKET)
	{
		return -1;
	}

	SOCKADDR_IN serverAddr;
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
	serverAddr.sin_port = htons(7777);

	if (::bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		return -1;
	}

	if (::listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		return -1;
	}

	cout << "ACCEPT" << endl;

	SOCKADDR_IN clientAddr;
	int32 addrLen = sizeof(clientAddr);

	while (true)
	{
		SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
		if (clientSocket == INVALID_SOCKET)
		{
			// 원래 블록했어야 하는데? 아니니깐
			if (::WSAGetLastError() == WSAEWOULDBLOCK)
			{
				continue;
			}

			// Error
			break;
		}

		cout << "Client Connected! " << endl;

		// Recv
		while (true)
		{
			char recvBuffer[1000];
			int32 recvLen = ::recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
			if (recvLen == SOCKET_ERROR)
			{
				// 원래 블록했어야 하는데? 아니니깐
				if (::WSAGetLastError() == WSAEWOULDBLOCK)
				{
					continue;
				}

				// Error
				break;
			}

			else if (recvLen == 0)
			{
				// 연결 끊김 
				break;
			}

			cout << "Recv Data Len = " << recvLen << endl;

			// Send
			while (true)
			{
				if (::send(clientSocket, recvBuffer, recvLen, 0) == SOCKET_ERROR)
				{
					// 원래 블록했어야 하는데? 아니니깐
					if (::WSAGetLastError() == WSAEWOULDBLOCK)
					{
						continue;
					}

					// Error
					break;
				}

				cout << "Send Data! Len = " << recvLen << endl;

				break;
			}
		}
	}

	// WinSock 종료
	::WSACleanup();
}