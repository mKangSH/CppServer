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

	// 소켓 리소스 반환
	::closesocket(clientSocket);

	// WinSock 종료
	::WSACleanup();
}