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
	// Initialize ws2_32 Library
	// 관련 정보가 wsaData에 채워짐
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		return -1;
		::WSACleanup();
	}

	// tcp => Socket 유실 시 프로토콜 단에서 따로 작업 없이 재전송 및 데이터 오염 체크
	// udp => 빠른 전송 유실 체크 x

	// ad : Address Family (AF_INET = IPv4, AF_INET6 = IPv6)
	// type : TCP(SOCK_STERAM) vs UDP(SOCK_DGRAM)
	// protocol : 0
	// return : descriptor
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

	// htons = host(local computer) to network short
	// Endian 이슈 때문에 사용 => Big-Endian(network) vs Little-Endian
	serverAddr.sin_port = ::htons(7777); // 80 : HTTP etc..
	
	// Server Address => DNS 교체
	// serverAddr.sin_addr.s_addr = ::inet_addr("127.0.0.1") << deprecated;
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

		this_thread::sleep_for(1s);
	}

	// ============================== //

	// 소켓 리소스 반환
	::closesocket(clientSocket);

	// WinSock 종료
	::WSACleanup();
}