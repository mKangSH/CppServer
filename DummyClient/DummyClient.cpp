#include "pch.h"
#include <iostream>

#include <WinSock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>
// 매우 중요
#pragma comment(lib, "ws2_32.lib")

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

	// TCP 서버는 Recv Buffer 전체를 읽어 Data 시작과 끝 부분을 알 수 없음 
	// => Packet 으로 데이터를 전송하여 시작과 끝을 알림

	SOCKET clientSocket = ::socket(AF_INET, SOCK_DGRAM, 0);
	if (clientSocket == INVALID_SOCKET)
	{
		// Log Viewer 제작 후 Log 띄우기
		HandleError("Socket");
		return -1;
	}

	// 127.0.0.1 Loop Back IP
	// Destination = (IP Addr + Port) 
	SOCKADDR_IN serverAddr; // IPv4
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = ::htons(7777);
	::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

	// Connected UDP(보낼 대상 즐겨찾기 동작 자체는 Unconnected UDP와 동일)
	// 실질적인 Connection X
	::connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr));

	// ============================== //
	// 연결 성공 ! 데이터 송수신 가능 !

	cout << "Connected To Server!" << endl;

	while (true)
	{
		// TODO 
		char sendBuffer[100] = "Hello World!";

		// 나의 IP 주소 + 포트 번호 설정 (tcp의 경우 connect에서 설정됨)
		
		// Unconnected UDP
		// int32 resultCode = ::sendto(clientSocket, sendBuffer, sizeof(sendBuffer), 0, 
		//						(SOCKADDR*)&serverAddr, sizeof(serverAddr));

		int32 resultCode = ::send(clientSocket, sendBuffer, sizeof(sendBuffer), 0);
		if (resultCode == SOCKET_ERROR)
		{
			HandleError("SendTo");
			return -1;
		}
		cout << "Send Data! Data = " << sendBuffer << endl;
		cout << "Send Data! Len = " << sizeof(sendBuffer) << endl;

		SOCKADDR_IN recvAddr;
		::memset(&recvAddr, 0, sizeof(recvAddr));
		int32 AddrLen = sizeof(recvAddr);

		char recvBuffer[1000];

		// Unconnected UDP
		// int32 recvLen = ::recvfrom(clientSocket, recvBuffer, sizeof(recvBuffer), 0,
		//						(SOCKADDR*)&recvAddr, &AddrLen);

		int32 recvLen = ::recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
		if (recvLen <= 0)
		{
			HandleError("RecvFrom");
			return 0;
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