#include "ThreadController.h"

ThreadController::ThreadController() {}

ThreadController::ThreadController(const uintptr_t uClient) {
	this->hDriver = CreateFile(L"\\\\.\\KM_KBRDCS2RWAPP", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, nullptr);
	this->uClient = uClient;
}

ThreadController::~ThreadController()
{
	for (ThreadedObject* ob : vCallstack)
		delete ob;

	CloseHandle(hDriver);
}

void ThreadController::Start(bool bRunning)
{
	while (bRunning)
		Update();
}

void ThreadController::Update()
{
	ThreadMgr::getInstance()->getMutex().lock();

	if (hDriver && uClient)
		for (ThreadedObject* ob : vCallstack)
		{
			ob->Update(hDriver, uClient);
		}

	std::this_thread::yield();
	ThreadMgr::getInstance()->getMutex().unlock();
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
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
