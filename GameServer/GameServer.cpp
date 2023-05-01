#include "pch.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <future>

int64 result;

int64 Calculate()
{
	int64 sum = 0;

	for (int32 i = 0; i < 1'000'000'000; i++)
	{
		sum += i;
	}

	result = sum;

	return sum;
}

class Knight
{
public:
	int64 GetHP() { return _HP; }
private:
	int64 _HP = 100;
};

void PromiseWorker(std::promise<string>&& promise)
{
	promise.set_value("Secret message!");
}

void TaskWorker(std::packaged_task<int64(void)>&& task)
{
	task();
}

int main()
{

	// 동기 (synchronous) 실행
	/*
	int64 sum = Calculate();
	cout << sum << endl;
	*/

	// 비동기 실행

	// thread 방식
	/*
	{
		
		thread t(Calculate);
		//
		// TODO
		//
		t.join();
		
	}
	*/
	
	// std::future : 중요도는 낮지만 시간이 오래걸리는 함수 처리에 유용
	{
		// async 옵션
		// 1) deferred -> lazy evaluation 지연 실행 (command pattern)
		// 2) async -> 별도의 쓰레드 생성하여 실행
		// 3) deferred | async -> 둘 중 컴파일러가 선택하여 실행

		// 미래에 결과물을 반드시 뱉는다는 의미
		std::future<int64> future1 = std::async(std::launch::async, Calculate);
		
		Knight knight;
		// 멤버 함수는 객체에 의존하기 때문에 객체도 포함하여 async 호출
		std::future<int64> future2 = std::async(std::launch::async, &Knight::GetHP, knight); // knight.GetHP();

		// TODO
		for (int i = 0; i < 1'000'000; i++)
		{
			this_thread::sleep_for(300ns);
		}

		cout << "knight HP: ";
		int64 knightHP = future2.get();
		cout << knightHP << endl;
		
		cout << "1부터 1,000,000,000까지의 합: ";
		int64 sum = future1.get(); // 완료될 때까지 기다려준다. 
		cout << sum << endl;
	}

	// std::promise
	{
		// 미래 결과물을 반환할거라 약속 [계약서]
		// promise는 서브 쓰레드로 future는 주 쓰레드에서 보유
		std::promise<string> promise1; 
		std::future<string> future1 = promise1.get_future();

		thread t(PromiseWorker, std::move(promise1));

		// TODO

		// future.get() 하고 난 이후엔 future1의 값이 null이 되어버림 2번 이상 호출 x
		string message1 = future1.get();
		cout << message1 << endl;

		t.join();
	}

	// std::packaged_task
	{
		std::packaged_task<int64(void)> task(Calculate);
		std::future<int64> future1 = task.get_future();

		thread t(TaskWorker, std::move(task));

		// TODO

		cout << "1부터 1,000,000,000까지의 합: ";
		int64 res = future1.get();
		cout << res << endl;

		t.join();
	}

	// 결론)
	// mutex, condition_variable까지 가지 않고 단순한 일을 처리할 때 사용 가능함
	// 특히 단발성 이벤트에 유용.

	// 1) async
	// 원하는 함수 비동기 실행

	// 2) promise
	// 결과물을 promise를 통해 future로 get

	// 3) packaged_task
	// 원하는 함수의 실행 결과를 packaged_task를 통해 future로 get
}