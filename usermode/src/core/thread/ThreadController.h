#pragma once
#include <Windows.h>
#include <chrono>

#include <vector>

#define MAX_CALLSTACK_SIZE 3

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

class ThreadController
{
private:
	std::vector<std::shared_ptr<ThreadElementController>> vCallStack;
public:
	ThreadController();
	~ThreadController();

	bool addThreadElement(ThreadElementController&);
	void Update();

	bool availible();
};