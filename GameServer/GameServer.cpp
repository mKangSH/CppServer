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

const int32 BUFSIZE = 1000;

struct Session
{
	SOCKET socket = INVALID_SOCKET;
	char recvBuffer[BUFSIZE] = {};
	int32 recvBytes = 0;
	WSAOVERLAPPED overlapped = {};
};

int main()
{
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		return -1;
	}

	SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET)
	{
		return -1;
	}

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

	cout << "Accept!" << endl;
	// AcceptEx
	// ConnectEx

	// Overlapped IO ( Async + Non-Blocking )
	// - Overlapped Function을 건다. (WSARecv, WSASend)
	// - Overlapped Function Success ? 
	// - Success => Processing
	// - Fail => 실패 사유를 확인

	// 1) 비동기 IO Socket
	// 2) WSABuf 배열의 시작 주소 및 개수 // Scatter-Gather 조각난 버퍼를 모아서 전송
	// 3) 송/수신 바이트 수
	// 4) 상세 옵션 => 0
	// 5) WSAOVERLAPPED 구조체 주소값
	// 6) 입출력 완료시 OS가 호출할 콜백 함수
	// WSASend 사용
	// WSARecv 사용

	// Overlapped Model (Event 기반)
	// - 비동기 입출력 지원하는 소켓 생성 + 통지 받기 위한 이벤트 객체 생성
	// - 비동기 입출력 함수 호출(1에서 만든 이벤트 객체를 같이 넘겨줌)
	// - 비동기 작업이 바로 완료되지 않는 경우 WSA_IO_PENDING 오류 코드
	// 운영체제는 이벤트 객체를 signaled 상태로 만들어 완료 상태 알려줌.
	// - WSAWaitForMultipleEvents 함수 호출해서 이벤트 객체의 signal 판별
	// - WSAGetOverlappedResult 호출해서 비동기 입출력 결과 확인 및 데이터 처리
	
	// 1) 비동기 소켓
	// 2) 넘겨준 overlapped 구조체
	// 3) 전송된 바이트 수
	// 4) 비동기 입출력 작업이 끝날때까지 대기할지 ?
	// false
	// 5) 비동기 입출력 작업 관련 부가 정보 거의 사용 안 함.

	while (true)
	{
		SOCKADDR_IN clientAddr;
		int32 addrLen = sizeof(clientAddr);

		SOCKET clientSocket;
		while (true)
		{
			clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);

			if (clientSocket != INVALID_SOCKET)
			{
				break;
			}

			if (::WSAGetLastError() == WSAEWOULDBLOCK)
			{
				continue;
			}

			// 문제 발생!
			return 0;
		}

		Session session = Session{ clientSocket };
		WSAEVENT wsaEvent = ::WSACreateEvent();
		session.overlapped.hEvent = wsaEvent;

		cout << "Client Connected!" << endl;

		// recvBuffer, overlapped 값은 동작이 완전히 끝날 때까지 건드리지 말아야 한다! 데이터 오염!
		while (true)
		{
			WSABUF wsaBuf;
			wsaBuf.buf = session.recvBuffer;
			wsaBuf.len = BUFSIZE;

			DWORD recvLen = 0;
			DWORD flags = 0;
			if (::WSARecv(clientSocket, &wsaBuf, 1, &recvLen, &flags, &session.overlapped, nullptr) == SOCKET_ERROR)
			{
				if (::WSAGetLastError() == WSA_IO_PENDING)
				{
					// Pending
					::WSAWaitForMultipleEvents(1, &wsaEvent, TRUE, WSA_INFINITE, FALSE);
					::WSAGetOverlappedResult(session.socket, &session.overlapped, &recvLen, FALSE, &flags);
				}

				else
				{
					// TODO : 문제 상황
					break;
				}
			}

			cout << "Data Recv Len = " << recvLen << endl;
		}

		::closesocket(session.socket);
		::WSACloseEvent(wsaEvent);
	}
	
	// WinSock 종료
	::WSACleanup();
}