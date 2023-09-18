#include "pch.h"
#include "Session.h"
#include "SocketUtils.h"
#include "Service.h"

//================================//
//			  Session			  //
//================================//

Session::Session()
{
	_socket = SocketUtils::CreateSocket();
}

Session::~Session()
{
	SocketUtils::Close(_socket);
}

void Session::Send(BYTE* buffer, int32 len)
{
	// 생각할 문제
	// 1) 버퍼 관리
	// 2) Send Event 관리, 단일? 여러개? WSASend 중첩?

	// temp
	SendEvent* sendEvent = xnew<SendEvent>();
	sendEvent->owner = shared_from_this();
	sendEvent->buffer.resize(len);
	::memcpy(sendEvent->buffer.data(), buffer, len);

	WRITE_LOCK
	RegisterSend(sendEvent);
}

void Session::Disconnect(const WCHAR* cause)
{
	if (_connected.exchange(false) == false)
	{
		return;
	}

	// TEMP 
	wcout << "Disconnect : " << cause << endl;

	// 컨텐츠 단에서 오버로딩
	OnDisconnected();

	SocketUtils::Close(_socket);
	GetService()->ReleaseSession(GetSessionRef());
}

HANDLE Session::GetHandle()
{
	return reinterpret_cast<HANDLE>(_socket);
}

void Session::Dispatch(IocpEvent* iocpEvent, int32 numOfBytes)
{
	switch (iocpEvent->eventType)
	{
	case EventType::Connect:
		ProcessConnect();
		break;

	case EventType::Recv:
		ProcessRecv(numOfBytes);
		break;

	case EventType::Send:
		ProcessSend(static_cast<SendEvent*>(iocpEvent), numOfBytes);
		break;

	default:

		break;
	}
}

void Session::RegisterConnect()
{
}

void Session::RegisterRecv()
{
	if (IsConnected() == false)
	{
		return;
	}

	// RecvEvent* recvEvent = xnew<RecvEvent>();
	// recvEvent->owner = shared_from_this();

	_recvEvent.Init();
	_recvEvent.owner = shared_from_this(); // ADD REF

	WSABUF wsaBuf;
	wsaBuf.buf = reinterpret_cast<char*>(_recvBuffer);
	wsaBuf.len = len32(_recvBuffer);

	DWORD numOfBytes = 0;
	DWORD flags = 0;
	if (SOCKET_ERROR == ::WSARecv(_socket, &wsaBuf, 1, OUT &numOfBytes, OUT &flags, &_recvEvent, NULL))
	{
		int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			HandleError(errorCode);
			_recvEvent.owner = nullptr; // RELEASE REF
		}
	}
}

void Session::RegisterSend(SendEvent* sendEvent)
{
	if (IsConnected() == false)
	{
		return;
	}

	// Page Lock (언제 어디서든 사용 가능하게 주시하면서 추가 작업하는 개념)
	WSABUF wsaBuf;
	wsaBuf.buf = (char*)sendEvent->buffer.data();
	wsaBuf.len = (ULONG)sendEvent->buffer.size();

	DWORD numOfBytes = 0;
	if (SOCKET_ERROR == ::WSASend(_socket, &wsaBuf, 1, OUT & numOfBytes, 0, sendEvent, nullptr))
	{
		int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			HandleError(errorCode);
			sendEvent->owner = nullptr; // RELEASE_REF
			xdelete(sendEvent);
		}
	}
}

void Session::ProcessConnect()
{
	_connected.store(true);

	// 세션 등록
	GetService()->AddSession(GetSessionRef());

	// 컨텐츠 코드에서 오버로딩
	OnConnected();

	// 수신 등록
	RegisterRecv();
}

void Session::ProcessRecv(int32 numOfBytes)
{
	_recvEvent.owner = nullptr; // RELEASE_REF

	if (numOfBytes == 0)
	{
		Disconnect(L"ProcessRecv Bytes 0");
		return;
	}

	// 컨텐츠 단에서 오버로딩
	OnRecv(_recvBuffer, numOfBytes);

	// 수신 등록 :
	RegisterRecv();
}

void Session::ProcessSend(SendEvent* sendEvent, int32 numOfBytes)
{
	sendEvent->owner = nullptr; // RELEASE_REF
	xdelete(sendEvent);

	if (numOfBytes == 0)
	{
		Disconnect(L"Send 0");
	}

	// 컨텐츠 코드에서 오버로딩
	OnSend(numOfBytes);
}

void Session::HandleError(int32 errorCode)
{
	switch (errorCode)
	{
	case WSAECONNRESET:
	case WSAECONNABORTED:
		Disconnect(L"Handle Error");
		break;

	default:
		// TODO : Log Thread 생성 후 일감 등록 방식으로 변경
		cout << "Handle Error : " << errorCode << endl;
		break;
	}
}
