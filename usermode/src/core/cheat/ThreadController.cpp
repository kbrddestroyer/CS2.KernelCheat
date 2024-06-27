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

void ThreadController::addElement(ThreadedObject* ob)
{
	vCallstack.push_back(ob);
}

void ThreadMgr::Start()
{
	if (!pController)
		pController = new ThreadController(hDriver, uClient);
	pController->Start();
}

ThreadMgr::ThreadMgr(HANDLE& hDriver, const uintptr_t& uClient) : ThreadMgr()
{
	this->hDriver = hDriver;
	this->uClient = uClient;

	thread = std::thread(&ThreadMgr::Start, this);
}
