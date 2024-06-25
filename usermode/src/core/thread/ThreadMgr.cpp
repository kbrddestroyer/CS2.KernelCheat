#include "ThreadMgr.h"

ThreadMgr::ThreadMgr()
{
	uLastUpdateTimestamp = time(0);
}

ThreadMgr::~ThreadMgr()
{

}

std::shared_ptr<ThreadController> ThreadMgr::createThread()
{
	std::shared_ptr<ThreadController> ptr = std::make_shared<ThreadController>();
	vThreads.push_back(ptr);

	return ptr;
}

ThreadController* ThreadMgr::getThread()
{
	for (std::shared_ptr<ThreadController> thController : vThreads)
	{
		if (thController->availible())
			return thController.get();
	}

	return createThread().get();
}