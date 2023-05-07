#include "pch.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <future>
#include <Windows.h>
#include "ConcurrentQueue.h"
#include "ConcurrentStack.h"

// 소수 구하기
const int32 MAX_NUMBER = 1'000'000;
mutex m;
vector<bool> primeNumbers(MAX_NUMBER + 1, true);

void IsPrime(int start, int end)
{
	for (uint64 number = start; number <= end; number++)
	{
		for (uint64 j = number * number; j <= MAX_NUMBER; j += number)
		{
			primeNumbers[j] = false;
		}
	}
}

int CountPrimeNumber(int start, int end)
{
	int count = 0;
	for (int i = start; i < end; i++)
	{
		if (primeNumbers[i])
		{
			count++;
		}
	}

	return count;
}

int main()
{
	vector<thread> threads;

	int coreCount = thread::hardware_concurrency();
	int jobCount = (sqrt(MAX_NUMBER) / coreCount) + 1;

	for (int i = 0; i < coreCount; i++)
	{
		int start = (i * jobCount);
		int end = min(MAX_NUMBER, ((i + 1) * jobCount));

		if (i == 0)
		{
			start = 2;
		}

		threads.push_back(thread([start, end]()
			{
				IsPrime(start, end);
			}
		));
	}

	for (thread& t : threads)
	{
		t.join();
	}

	jobCount = (MAX_NUMBER / coreCount) + 1;
	int primeCount = 0;
	for (int i = 0; i < coreCount; i++)
	{
		int start = (i * jobCount);
		int end = min(MAX_NUMBER, ((i + 1) * jobCount));

		if (i == 0)
		{
			start = 2;
		}

		threads[i] = thread([start, end, &primeCount]()
			{
				primeCount += CountPrimeNumber(start, end);
			}
		);
	}

	for (thread& t : threads)
	{
		t.join();
	}

	cout << primeCount << endl;
}