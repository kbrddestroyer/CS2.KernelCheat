#pragma once
#include <thread>
#include <future>
#include <mutex>
#include <vector>

#include <Windows.h>

class IThreadController;
class ThreadedObject;
typedef std::shared_ptr<IThreadController> PThreadController;
typedef std::shared_ptr<ThreadedObject> PThreadedObject;

class ThreadedObject
{
public:
	static bool createObject(PThreadedObject);
	virtual void Update(HANDLE hDriver, uintptr_t uClient) = 0;
};

class IThreadController
{
private:
	std::unique_ptr<std::thread> thControl;
protected:
	bool bRunning = true;
public:
	~IThreadController();

	void openThread();

	std::thread* getThreadRef() { return thControl.get(); }

	virtual void Start(bool) = 0;
	virtual void Update() = 0;
};

class ThreadController : public IThreadController
{
private:
	HANDLE		hDriver;
	uintptr_t	uClient;
	const uint32_t	uMaximum = 3;
	uint32_t	id = 0;
	std::vector<PThreadedObject> vCallstack;
private:
	ThreadController();
public:
	ThreadController(const uintptr_t, uint32_t);
	~ThreadController();

	void setClient(uintptr_t uClient) { this->uClient = uClient; }

	void Start(bool) override;
	void Update() override;
	
	void resize();
	void addElement(PThreadedObject ob);

	bool isAvailible() { return vCallstack.size() < uMaximum; }
};

class ThreadMgr
{
private:
	inline static ThreadMgr* instance;

	std::vector<PThreadController> vControllers;

	HANDLE		hDriver;
	uintptr_t	uClient;

	std::mutex mtx;
public:
	ThreadMgr();
	~ThreadMgr();

	std::mutex&			getMutex()	{ return mtx; }
	HANDLE				DRIVER()	{ return hDriver; }
	uintptr_t			CLIENT()	{ return uClient; }
	ThreadController*	getThread();
	void				setKMParams(HANDLE, uintptr_t);

#pragma region ThreadMgr loop controller
	void Start();
	void AddThreadController(PThreadController);
#pragma endregion

	static ThreadMgr* getInstance() { return instance; }
};