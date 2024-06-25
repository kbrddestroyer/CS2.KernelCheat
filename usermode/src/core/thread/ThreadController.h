#pragma once
#include <Windows.h>
#include <chrono>

class ThreadElementController
{
private:
public:
};

class ThreadElementController
{
protected:
	unsigned int uUpdateDeltaTime = 0;		// 0 is fastest possible
public:
	ThreadElementController() {}
	~ThreadElementController() {}

	virtual void Update() = 0;
};

