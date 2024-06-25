#pragma once
#include <thread>
#include <chrono>
#include <vector>

#include "ThreadController.h"

/*
*	Todo:
* - Create dynamic thread managed w/ thread stack size control and automatic extention to new thread
*/

class ThreadMgr
{
private:
	uint32_t uLastUpdateTimestamp;
protected:
	std::vector<std::shared_ptr<ThreadController>> vThreads;
public:
	ThreadMgr();
	~ThreadMgr();

	void createThread(void* func(...));
};

