#pragma once
#include <thread>
#include <future>
#include <vector>

#include <Windows.h>

class ThreadedObject
{
public:
	virtual void Update(HANDLE hDriver, uintptr_t uClient) = 0;
};

class IThreadController
{
private:
	std::unique_ptr<std::thread> thControl;
	bool bRunning = true;
public:
	~IThreadController()
	{
		bRunning = false;
	}

	void openThread()
	{
		thControl = std::make_unique<std::thread>(&IThreadController::Start, this, std::ref(bRunning));
	}

	std::thread* getThreadRef() { return thControl.get(); }

	virtual void Start(bool) = 0;
	virtual void Update() = 0;
	void Stop() { bRunning = false; }
};

class ThreadController : public IThreadController
{
/*
*	class is used for cheat controls
*/
private:
	HANDLE		hDriver;
	uintptr_t	uClient;
	std::vector<ThreadedObject*> vCallstack;
public:
	ThreadController();
	ThreadController(const uintptr_t);
	~ThreadController();

	void setClient(uintptr_t uClient) { this->uClient = uClient; }
	void Start(bool) override;
	void Update() override;

	void addElement(ThreadedObject* ob);
};

class ThreadMgr
{
private:
	inline static ThreadMgr* instance;

	std::vector<IThreadController*> vControllers;

	HANDLE		hDriver;
	uintptr_t	uClient;
public:
	ThreadMgr() { instance = this; }
	ThreadMgr(HANDLE&, const uintptr_t&);

	~ThreadMgr()
	{
		for (IThreadController* controller : vControllers)
			if (controller)
				delete controller;
		vControllers.clear();
	}

	ThreadController* getThread() {
		for (IThreadController* controller : vControllers)
		{
			if (dynamic_cast<ThreadController*>(controller))
			{
				return reinterpret_cast<ThreadController*>(controller);
			}
		}
		IThreadController* controller = new ThreadController(uClient);

		controller->openThread();
		vControllers.push_back(controller);
		return reinterpret_cast<ThreadController*>(controller);
	}

	void params(HANDLE hDriver, uintptr_t uClient)
	{
		this->hDriver = hDriver;
		this->uClient = uClient;
	}

	HANDLE DRIVER() { return hDriver; }
	uintptr_t CLIENT() { return uClient; }

	void Start();
	
	void add(IThreadController*);
	static ThreadMgr* getInstance() { return instance; }
};