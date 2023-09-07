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
	WSAOVERLAPPED overlapped = {};
	SOCKET socket = INVALID_SOCKET;
	char recvBuffer[BUFSIZE] = {};
	int32 recvBytes = 0;
};

void CALLBACK RecvCallback(DWORD error, DWORD recvLen, LPWSAOVERLAPPED overlapped, DWORD flags)
{
	cout << "Data Recv Len Callback = " << recvLen << endl;
	// TODO : ex) 에코 서버를 만든다면 WSASend();

	Session* session = (Session*)overlapped;
}

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
	
	// Overlapped Model (Completion Routine 콜백 기반)
	// - 비동기 입출력 지원하는 소켓 생성
	// - 비동기 입출력 함수 호출(완료 루틴의 시작 주소를 넘겨준다.)
	// - 비동기 작업이 바로 완료되지 않는 경우 WSA_IO_PENDING 오류 코드
	// - 비동기 입출력 함수 호출한 쓰레드를 ->Alertable Wait 상태로 만든다.
	// ex) WaitForSingleObject, WaitForMultipleObjectEx, SleepEx, WSAWaitForMultipleEvents
	// - 비동기 IO 완료되면, 운영체제는 완료 루틴 호출 (C#에서는 비동기 작업이 완료되면 운영체제가 Thread pool에 있는 Thread를 호출해서 완료)
	// - 완료 루틴 호출이 완료되면, 쓰레드는 Alertable Wait 상태에서 빠져나온다.
	// Lock 상황에서 비동기 IO Callback이 호출되면 ??
	
	// 1) 오류 발생 시 0이 아닌 값
	// 2) 전송 바이트 수
	// 3) 비동기 입출력 함수 호출 시 넘겨준 WSAOVERLAPPED 구조체의 주소값
	// 4) 0
	// void CompletionRoutine()

	// Select 모델
	// - 장점) 윈도우/리눅스 공통(Cross Platform)
	// - 단점) 성능 최하 (매번 소켓을 등록하는 비용), 64개 제한
	// WSAAsyncSelect 모델 = 소켓 이벤트를 윈도우 메시지 형태로 처리(일반 윈도우 메시지랑 같이 처리하니 성능 --)
	// WSAEventSelect 모델
	// - 장점) 비교적 뛰어난 성능
	// - 단점) 64개 제한
	// Overlapped (이벤트 기반)
	// - 장점) 성능
	// - 단점) 64개 제한
	// Overlapped (콜백 기반)
	// - 장점) 성능
	// - 단점) 모든 비동기 소켓 함수에서 사용 가능하진 않음(Accept 등), 빈번한 Alertable Wait으로 인한 성능 저하
	// IOCP

	// Reactor Pattern (-뒤늦게, Non-Blocking 소켓, 소켓 상태 확인 후 -> 뒤늦게 호출)
	// Proactor Pattern (-미리, Overlapped WSA~ 걸어놓고 알아서 처리하게 한다)

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
		//WSAEVENT wsaEvent = ::WSACreateEvent();

		cout << "Client Connected!" << endl;

		// recvBuffer, overlapped 값은 동작이 완전히 끝날 때까지 건드리지 말아야 한다! 데이터 오염!
		while (true)
		{
			WSABUF wsaBuf;
			wsaBuf.buf = session.recvBuffer;
			wsaBuf.len = BUFSIZE;

			DWORD recvLen = 0;
			DWORD flags = 0;
			if (::WSARecv(clientSocket, &wsaBuf, 1, &recvLen, &flags, &session.overlapped, RecvCallback) == SOCKET_ERROR)
			{
				if (::WSAGetLastError() == WSA_IO_PENDING)
				{
					// Pending
					// Alertable Wait (APC)

					::SleepEx(INFINITE, TRUE);
					// ::WSAWaitForMultipleEvents(1, &wsaEvent, TRUE, WSA_INFINITE, TRUE);
				}

				else
				{
					// TODO : 문제 상황
					break;
				}
			}

			else
			{
				cout << "Data Recv Len = " << recvLen << endl;
			}
		}

		::closesocket(session.socket);
		//::WSACloseEvent(wsaEvent);
	}
	
	// WinSock 종료
	::WSACleanup();
}