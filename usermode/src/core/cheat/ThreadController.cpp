#include "ThreadController.h"

ThreadController::ThreadController() {}

ThreadController::ThreadController(HANDLE& hDriver, const uintptr_t& uClient) {
	this->hDriver = hDriver;
	this->uClient = uClient;
}

void ThreadController::Start()
{
	while (true)
	{
		Update();
	}
}

void ThreadController::Update()
{
	for (ThreadedObject* ob : vCallstack)
	{
		ob->Update(hDriver, uClient);
	}
}

ThreadMgr::ThreadMgr(HANDLE& hDriver, const uintptr_t& uClient)
{
	this->hDriver = hDriver;
	this->uClient = uClient;
}

void ThreadMgr::Start()
{
	ThreadController controller(hDriver, uClient);

	controller.Start();
}