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
	// Initialize ws2_32 Library
	// 관련 정보가 wsaData에 채워짐
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		return -1;
	}

	SOCKET serverSocket = ::socket(AF_INET, SOCK_DGRAM, 0);
	if (serverSocket == INVALID_SOCKET)
	{
		HandleError("Socket");
		return -1;
	}

	// 옵션을 해석하고 처리할 주체 -> Level
	//    level -> KEEPALIVE, LINGER, RCVBUF, REUSEADDR, SNDBUF 공부해보기
	// 소켓 코드 -> SOL_SOCKET
	// IPv4 -> IPPROTO_IP
	// TCP 프로토콜 -> IPPROTO_IP

	// SO_KEEPALIVE = 주기적인 연결상태 확인 여부(TCP Only)
	// 상대방이 연결을 일방적으로 통보 없이 끊은 경우
	// 주기적으로 TCP 프로토콜 연결 상태 확인 -> 감지하고 싶을 경우 KEEPALIVE 사용

	bool enable = true;
	::setsockopt(serverSocket, SOL_SOCKET, SO_KEEPALIVE, (char*)&enable, sizeof(enable));

	// SO_LINGER = 지연하다
	// 송신 버퍼에 있는 데이터를 보낼 것인가? 날릴 것인가?

	// linger.l_onoff = 0이면 closesocket() 바로 리턴, 아니면 linger초만큼 대기 (default 0)
	// linger.l_linger : 대기 시간
	LINGER linger;
	linger.l_onoff = 1;
	linger.l_linger = 5;
	::setsockopt(serverSocket, SOL_SOCKET, SO_LINGER, (char*)&linger, sizeof(linger));


	// Half-Close
	// SD_SEND : send 막는다
	// SD_RECEIVE : recv 막는다
	// SD_BOTH : 둘 다 막는다
	// ::shutdown(serverSocket, SD_SEND);

	// 소켓 리소스 반환
	// if send -> closesocket 호출된 경우
	// ::closesocket(serverSocket);

	// SO_RCVBUF = 수신 버퍼 크기
	int32 recvBufferSize;
	int32 optionLength = sizeof(recvBufferSize);
	::getsockopt(serverSocket, SOL_SOCKET, SO_RCVBUF, (char*)&recvBufferSize, &optionLength);
	cout << "수신 버퍼 크기 : " << recvBufferSize << endl;

	// SO_SNDBUF = 송신 버퍼 크기
	int32 sendBufferSize;
	optionLength = sizeof(sendBufferSize);
	::getsockopt(serverSocket, SOL_SOCKET, SO_SNDBUF, (char*)&sendBufferSize, &optionLength);
	cout << "송신 버퍼 크기 : " << sendBufferSize << endl;

	// SO_REUSEADDR
	// IP 주소 및 Port 재사용
	// 주소가 사용 중이거나, 강제 종료 이후 데이터가 남아있는 경우 => 바인딩 실패
	// 해당 사항 예방차원으로 REUSEADDR 설정
	{
		bool enable = true;
		::setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&enable, sizeof(enable));
	}

	// IPPROTO_TCP <- socket 이 아닌 tcp 단계에서 설정
	// TCP_NODELAY = Nagle 네이글 알고리즘 작동 여부
	// 데이터가 충분히 크면 보내고, 그렇지 않으면 충분히 쌓일 때까지 대기
	// 장점 : 작은 패킷 불필요하게 생성되는 일을 방지
	// 단점 : 반응 시간 손해 
	// 게임과 같은 경우 반응 시간이 중요하기 때문에 NoDelay 설정
	{
		bool enable = true;
		::setsockopt(serverSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&enable, sizeof(enable));
	}


	// WinSock 종료
	::WSACleanup();
}