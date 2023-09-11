#pragma once

//=============================//
//		 IocpObject(key정보)	   //
//=============================//
class IocpObject : public enable_shared_from_this<IocpObject>
{
public:
	virtual HANDLE GetHandle() abstract;
	virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) abstract;

	// iocpEvent는 GetQueuedCompletionStatus 에서 overlapped 상속받아 정보를 넘기는 역할
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

	bool	Register(IocpObjectRef iocpObject);
	bool	Dispatch(uint32 timeoutMs = INFINITE);

private:
	HANDLE	_iocpHandle;
};