#include "ThreadController.h"

ThreadController::ThreadController() {}

ThreadController::ThreadController(HANDLE hDriver, const uintptr_t uClient) {
	this->hDriver = hDriver;
	this->uClient = uClient;
}

ThreadController::~ThreadController()
{
	for (ThreadedObject* ob : vCallstack)
		delete ob;
}

void ThreadController::Start()
{
	Update();
}

void ThreadController::Update()
{
	if (hDriver && uClient)
		for (ThreadedObject* ob : vCallstack)
		{
			ob->Update(hDriver, uClient);
		}
}

void ThreadController::addElement(ThreadedObject* ob)
{
	vCallstack.push_back(ob);
}

void ThreadMgr::Start()
{
	for (IThreadController* controller : vControllers)
		controller->openThread();
}

void ThreadMgr::Update()
{
	for (IThreadController* controller : vControllers)
	{
		controller->openThread();
		controller->getThreadRef()->join();
	}
}

void ThreadMgr::add(IThreadController* controller)
{
	controller->openThread();
	vControllers.push_back(controller);
}

ThreadMgr::ThreadMgr(HANDLE& hDriver, const uintptr_t& uClient) : ThreadMgr()
{
	this->hDriver = hDriver;
	this->uClient = uClient;
}
