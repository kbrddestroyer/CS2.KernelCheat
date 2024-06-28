#pragma once
#include <thread>
#include <vector>

#include <Windows.h>

class ThreadedObject
{
public:
	virtual void Update(HANDLE hDriver, uintptr_t uClient) = 0;
};

class ThreadController
{
private:
	HANDLE		hDriver;
	uintptr_t	uClient;

	std::vector<ThreadedObject*> vCallstack;
public:
	ThreadController();
	ThreadController(HANDLE, const uintptr_t);
	~ThreadController();

	void Start();
	void Update();

	void addElement(ThreadedObject* ob);
};

class ThreadMgr
{
private:
	std::vector<std::thread> threads;
	inline static ThreadMgr* instance;
	ThreadController* pController;

	HANDLE		hDriver;
	uintptr_t	uClient;
public:
	ThreadMgr() { instance = this; }
	ThreadMgr(HANDLE&, const uintptr_t&);
	~ThreadMgr() { if (pController) delete pController; }
	ThreadController* getThread() { 
		if (!pController)
			pController = new ThreadController(hDriver, uClient);
		return pController; 
	}
	void params(HANDLE hDriver, uintptr_t uClient)
	{
		this->hDriver = hDriver;
		this->uClient = uClient;
	}
	void Start();
	void Join()
	{
		thread = std::thread(&ThreadMgr::Start, this);
		thread.join();
	}

	static ThreadMgr* getInstance() { return instance; }
};