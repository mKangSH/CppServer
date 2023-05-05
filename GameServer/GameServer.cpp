#include "pch.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <future>
#include <Windows.h>

atomic<bool> ready = false;
int32 value;

void Producer()
{
	value = 10;

	ready.store(true, memory_order::memory_order_seq_cst);
}

void Consumer()
{
	while (ready.load(memory_order::memory_order_seq_cst) == false)
	{
		;
	}

	cout << value << endl;
}
int main()
{
	ready = false;
	value = 0;

	thread t1(Producer);
	thread t2(Consumer);

	t1.join();
	t2.join();
}
