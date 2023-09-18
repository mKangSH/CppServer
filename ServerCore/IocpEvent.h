#pragma once

class Session;

enum class EventType : uint8
{
	Connect,
	Accept,
	// PreRecv, 0 Byte Receive 기법과 연관됨 
	Recv,
	Send,
};

//==============================//
//			IocpEvent			//
//==============================//

// OVERLAPPED를 상속으로 구현하고 Event를 따로 메인Iocp Event를 상속받아 관리하는 경우
// 조심해야 하는 사항! 
// virtual 함수를 사용하지 마라! Why?
// virtual 함수를 사용하면 가상함수 테이블이 Offset 0번 Table에 들어가게 되면서
// Offset 0번에 Overlapped 관련된 메모리가 아닌 다른 메모리로 채워질 가능성 있음!
class IocpEvent : public OVERLAPPED
{
	// OVERLAPPED overlapped;

public:
	IocpEvent(EventType type);

	void Init();

public:
	IocpObjectRef	owner;
	EventType		eventType;
};

//==============================//
//		   ConnectEvent			//
//==============================//

class ConnectEvent : public IocpEvent
{
public:
	ConnectEvent() : IocpEvent(EventType::Connect) {}

private:

};

//==============================//
//			AcceptEvent			//
//==============================//

class AcceptEvent : public IocpEvent
{
public:
	AcceptEvent() : IocpEvent(EventType::Accept) {}

public:
	// TODO : 추가 정보 들고 있어야 함. 인자를 가지고 있는 생성자를 쓸 수도 있음.
	// Session 정보를 바인딩 Client Session
	SessionRef	session = nullptr;
};

//==============================//
//			RecvEvent			//
//==============================//

class RecvEvent : public IocpEvent
{
public:
	RecvEvent() : IocpEvent(EventType::Recv) {}

private:

};

//==============================//
//			SendEvent			//
//==============================//

class SendEvent : public IocpEvent
{
public:
	SendEvent() : IocpEvent(EventType::Send) {}

	// TEMP
	vector<BYTE> buffer;
private:

};