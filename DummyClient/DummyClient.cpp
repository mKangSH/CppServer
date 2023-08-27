#include "pch.h"
#include <iostream>

#include <WinSock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>
// 매우 중요
#pragma comment(lib, "ws2_32.lib")

// WSAStartup <=> WSACleanup
int main()
{
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		return -1;
		::WSACleanup();
	}

	// TCP 서버는 Recv Buffer 전체를 읽어 Data 시작과 끝 부분을 알 수 없음 
	// => Packet 으로 데이터를 전송하여 시작과 끝을 알림

	SOCKET clientSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET)
	{
		// Log Viewer 제작 후 Log 띄우기
		int32 errCode = ::WSAGetLastError();
		cout << "Socket ErrorCode : " << errCode << endl;
		::WSACleanup();
		return -1;
	}

	// 127.0.0.1 Loop Back IP
	// Destination = (IP Addr + Port) 
	SOCKADDR_IN serverAddr; // IPv4
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = ::htons(7777);
	::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr); 

	// 실질적인 연결 시도부
	if (::connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		// Log Viewer 제작 후 Log 띄우기
		int32 errCode = ::WSAGetLastError();
		cout << "Socket ErrorCode : " << errCode << endl;
		::WSACleanup();
		return -1;
	}

	// ============================== //
	// 연결 성공 ! 데이터 송수신 가능 !

	cout << "Connected To Server!" << endl;

	while (true)
	{
		// TODO 
		char sendBuffer[100] = "Hello World!";

		// send, recv => Blocking Function
		// send => kernal Level Send buffer가 꽉 찼을 때 block
		// recv => kernal Level Recv buffer가 꽉 찼을 때 block
		int32 resultCode = ::send(clientSocket, sendBuffer, sizeof(sendBuffer), 0);
		if (resultCode == SOCKET_ERROR)
		{
			int32 errCode = ::WSAGetLastError();
			cout << "Send ErrorCode : " << errCode << endl;
			::WSACleanup();
			return -1;
		}

		cout << "Send Data! Len = " << sizeof(sendBuffer) << endl;

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

		this_thread::sleep_for(1s);
	}

	// ============================== //

	// 소켓 리소스 반환
	::closesocket(clientSocket);

	// WinSock 종료
	::WSACleanup();
}