#pragma once

//=============================//
//		 IocpObject(key����)	   //
//=============================//
class IocpObject
{
public:
	virtual HANDLE GetHandle() abstract;
	virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) abstract;

	// iocpEvent�� GetQueuedCompletionStatus ���� overlapped ��ӹ޾� ������ �ѱ�� ����
};

//=============================//
//			IocpCore		   //
//=============================//

class IocpCore
{
public:
	IocpCore();
	~IocpCore();

	HANDLE	GetIocpHandle() { return _iocpHandle; }

	bool	Register(class IocpObject* iocpObject);
	bool	Dispatch(uint32 timeoutMs = INFINITE);

private:
	HANDLE	_iocpHandle;
};

// TEMP
extern IocpCore GIocpCore;