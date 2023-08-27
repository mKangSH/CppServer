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

int main()
{
	// Initialize ws2_32 Library
	// 관련 정보가 wsaData에 채워짐
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		return -1;
		::WSACleanup();
	}

	SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET)
	{
		// Log Viewer 제작 후 Log 띄우기
		int32 errCode = ::WSAGetLastError();
		cout << "Socket ErrorCode : " << errCode << endl;
		::WSACleanup();
		return -1;
	}

	SOCKADDR_IN serverAddr; // IPv4
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = ::htons(7777); // 80 : HTTP etc..
	serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);

	if(::bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		// Log Viewer 제작 후 Log 띄우기
		int32 errCode = ::WSAGetLastError();
		cout << "Bind ErrorCode : " << errCode << endl;
		::WSACleanup();
		return -1;
	}

	const int backLog = 10; // 대기열 Capacity
	if (::listen(listenSocket, backLog) == SOCKET_ERROR)
	{
		// Log Viewer 제작 후 Log 띄우기
		int32 errCode = ::WSAGetLastError();
		cout << "Listen ErrorCode : " << errCode << endl;
		::WSACleanup();
		return -1;
	}

	// ================================================
	// Data 송수신 시작!

	while (true)
	{
		SOCKADDR_IN clientAddr; // IPv4 IP Ban 등에 사용하게 될 중요한 정보!
		::memset(&clientAddr, 0, sizeof(serverAddr));
		int32 addrLen = sizeof(clientAddr);

		SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
		if (clientSocket == INVALID_SOCKET)
		{
			// Log Viewer 제작 후 Log 띄우기
			int32 errCode = ::WSAGetLastError();
			cout << "Accept ErrorCode : " << errCode << endl;
			::WSACleanup();
			return -1;
		}

		// client Data 수신 완료!
		
		// IP 찍어보고 싶다!
		char ipAddress[16];
		::inet_ntop(AF_INET, &clientAddr.sin_addr, ipAddress, sizeof(ipAddress));
		cout << "Client Entry! IP = " << ipAddress << endl;

		// TODO
		while (true)
		{
			char recvBuffer[1000];

			int32 recvLen = ::recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
			if (recvLen <= 0)
			{
				int32 errCode = ::WSAGetLastError();
				cout << "Recv ErrorCode : " << errCode << endl;
				::WSACleanup();
				return -1;
			}

			cout << "Recv Data! Data = " << recvBuffer << endl;
			cout << "Recv Data! Len = " << recvLen << endl;

			int32 resultCode = ::send(clientSocket, recvBuffer, recvLen, 0);
			if (resultCode == SOCKET_ERROR)
			{
				int32 errCode = ::WSAGetLastError();
				cout << "Send ErrorCode : " << errCode << endl;
				::WSACleanup();
				return -1;
			}
		}
	}

	// ===============================================

	// WinSock 종료
	::WSACleanup();
}