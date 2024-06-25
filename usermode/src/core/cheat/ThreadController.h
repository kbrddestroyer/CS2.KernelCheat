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
	ThreadController(HANDLE&, const uintptr_t&);

	void Start();
	void Update();

	void addElement(ThreadedObject* ob);
};

class ThreadMgr
{
private:
	std::thread thread;

	HANDLE		hDriver;
	uintptr_t	uClient;
public:
	inline ThreadMgr() {}
	ThreadMgr(HANDLE&, const uintptr_t&);

	void Start();
};