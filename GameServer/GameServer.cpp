#include "pch.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <Windows.h>

mutex m;
queue<int32> q;
HANDLE handle = NULL;

// User-Level Object(표준 mutex와 짝지어 동작)
condition_variable cv;

void Producer()
{
}

void Consumer()
{
}


int main()
{
}