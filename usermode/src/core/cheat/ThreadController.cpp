#include "ThreadController.h"
#include "../cheat/Cheat.h"

#pragma region ThreadedObject

bool ThreadedObject::createObject(PThreadedObject pInstance)
{
	if (!ThreadMgr::getInstance())
		return false;

	ThreadMgr::getInstance()->getThread()->addElement(pInstance);
	return true;
}

#pragma endregion

#pragma region IThreadController

IThreadController::~IThreadController()
{
	Stop();
}

void IThreadController::openThread()
{
	thControl = std::make_unique<std::thread>(&IThreadController::Start, this);
}

void IThreadController::Stop()
{
	if (!bRunning)
		return;
	bRunning = false;

	thControl->join();
	thControl->~thread();
}
#pragma endregion

#pragma region ThreadController

ThreadController::ThreadController() {
	this->hDriver = nullptr;
	this->uClient = 0;
	this->id = 0;

	throw "ThreadController was created by default constructor";
}

ThreadController::ThreadController(const uintptr_t uClient, uint32_t id=0) {
	this->hDriver = CreateFile(L"\\\\.\\KM_KBRDCS2RWAPP", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, nullptr);
	this->uClient = uClient;
	this->id = id;
}

ThreadController::~ThreadController()
{
	Stop();
}

void ThreadController::Start()
{
	while (bRunning)
		Update();
}

void ThreadController::Update()
{
	if (hDriver && uClient && ThreadMgr::getInstance())
	{
		const std::vector<PThreadedObject>& callstack = vCallstack;

		for (PThreadedObject ob : callstack)
		{
			ob->Update(hDriver, uClient);
		}
	}
	std::this_thread::yield();
}

void ThreadController::Stop()
{
	if (!hDriver)
		return;
	vCallstack.clear();
	uClient = 0;
	IThreadController::Stop();
	CloseHandle(hDriver);

	hDriver = nullptr;
}

void ThreadController::resize()
{

}

void ThreadController::addElement(PThreadedObject ob)
{
	vCallstack.push_back(ob);
}

#pragma endregion

#pragma region ThreadMgr

ThreadMgr::ThreadMgr()
{
	instance = this;
	this->hDriver = nullptr;
	this->uClient = 0;
}

ThreadMgr::~ThreadMgr() 
{
	for (PThreadController pThreadController : vControllers)
	{
		pThreadController->Stop();
	}

	vControllers.clear();
	exit(0);
}

/*
@return Returns first availible ThreadController. Use for one-time calls only
Do not ever store return value
*/
ThreadController* ThreadMgr::getThread()
{
	for (PThreadController controller : vControllers)
	{
		ThreadController* thController = dynamic_cast<ThreadController*>(controller.get());
		if (thController && thController->isAvailible())
		{
			return thController;
		}
	}
	PThreadController controller = std::make_shared<ThreadController>(uClient);
	AddThreadController(controller);
	return dynamic_cast<ThreadController*>(controller.get());
}

void ThreadMgr::setKMParams(HANDLE hDriver, uintptr_t uClient)
{
	if (!hDriver || !uClient)
		throw "Invalid handles";

	this->hDriver = hDriver;
	this->uClient = uClient;
}

#pragma region ThreadMgr loop controller

void ThreadMgr::Start()
{
	for (PThreadController controller : vControllers)
		controller->openThread();
}

void ThreadMgr::AddThreadController(PThreadController controller)
{
	controller->openThread();
	vControllers.push_back(controller);
}

#pragma endregion

#pragma endregion