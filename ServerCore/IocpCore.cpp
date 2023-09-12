#include "pch.h"
#include "IocpCore.h"
#include "IocpEvent.h"

IocpCore::IocpCore()
{
    _iocpHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
    ASSERT_CRASH(_iocpHandle != INVALID_HANDLE_VALUE);
}

IocpCore::~IocpCore()
{
    ::CloseHandle(_iocpHandle);
}

bool IocpCore::Register(IocpObjectRef iocpObject)
{
    return ::CreateIoCompletionPort(iocpObject->GetHandle(), _iocpHandle, /*key*/0, 0);
}

// Worker Thread가 일감 체크하는 부분
bool IocpCore::Dispatch(uint32 timeoutMs)
{
    DWORD numOfBytes = 0;
    ULONG_PTR key = 0;
    IocpEvent* iocpEvent = nullptr;

    if (::GetQueuedCompletionStatus(_iocpHandle, OUT &numOfBytes, OUT &key, OUT reinterpret_cast<LPOVERLAPPED*>(&iocpEvent), timeoutMs))
    {
        IocpObjectRef iocpObject = iocpEvent->owner;
        iocpObject->Dispatch(iocpEvent, numOfBytes);
        // Listener RegisterAccpet에서 iocpCore에 session을 등록했기 때문에
        // Recv, Send 등의 동작을 하면 Session->Dispatch로 넘어감
    }

    else
    {
        int32 errCode = ::WSAGetLastError();

        switch (errCode)
        {
        case WAIT_TIMEOUT:
            cout << "Time Out Error" << endl;
            return false;

        default:
            // TODO : Log 찍기
            IocpObjectRef iocpObject = iocpEvent->owner;
            iocpObject->Dispatch(iocpEvent, numOfBytes);
            break;
        }
    }
    
}