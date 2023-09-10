#pragma once

class Session;

enum class EventType : uint8
{
	Connect,
	Accept,
	// PreRecv, 0 Byte Receive ����� ������ 
	Recv,
	Send,
};

//==============================//
//			IocpEvent			//
//==============================//

// OVERLAPPED�� ������� �����ϰ� Event�� ���� ����Iocp Event�� ��ӹ޾� �����ϴ� ���
// �����ؾ� �ϴ� ����! 
// virtual �Լ��� ������� ����! Why?
// virtual �Լ��� ����ϸ� �����Լ� ���̺��� Offset 0�� Table�� ���� �Ǹ鼭
// Offset 0���� Overlapped ���õ� �޸𸮰� �ƴ� �ٸ� �޸𸮷� ä���� ���ɼ� ����!
class IocpEvent : public OVERLAPPED
{
	// OVERLAPPED overlapped;

public:
	IocpEvent(EventType type);

	void		Init();
	EventType	GetType() { return _type; }

private:
	EventType	_type;

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

	void		SetSession(Session* session) { _session = session; }
	Session*	GetSession() { return _session; }

private:
	// TODO : �߰� ���� ��� �־�� ��. ���ڸ� ������ �ִ� �����ڸ� �� ���� ����.
	// Session ������ ���ε� Client Session
	Session*	_session = nullptr;
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

private:

};