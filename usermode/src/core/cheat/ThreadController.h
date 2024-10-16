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
protected:
	HANDLE hDriver = nullptr;
	uintptr_t uClient = 0;
	bool bRunning = true;
public:
	static bool createObject(PThreadedObject);
	virtual void createContext(HANDLE, uintptr_t);
	
	virtual void Update() = 0;
	
	void kill() { bRunning = false; }
	bool isKilled() { return !bRunning; }
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

	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void Stop();
};

class ThreadController : public IThreadController
{
private:
	HANDLE		hDriver;
	uintptr_t	uClient;
	const uint32_t	uMaximum = 1;
	uint32_t	id = 0;
	std::vector<PThreadedObject> vCallstack;
private:
	ThreadController();
public:
	ThreadController(const uintptr_t, uint32_t);
	~ThreadController();

	void setClient(uintptr_t uClient) { this->uClient = uClient; }

	void Start()	override;
	void Update()	override;
	void Stop()		override;
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
	bool		bDebug;

	std::mutex mtx;
public:
	ThreadMgr();
	~ThreadMgr();
	HANDLE				DRIVER()	{ return hDriver; }
	uintptr_t			CLIENT()	{ return uClient; }
	bool				DEBUG()		{ return bDebug; }
	ThreadController*	getThread();
	void				setKMParams(HANDLE, uintptr_t);
	void				setDebug(bool bDebug) { this->bDebug = bDebug; }
#pragma region ThreadMgr loop controller
	void Start();
	void AddThreadController(PThreadController);
#pragma endregion

	static ThreadMgr* getInstance() { return instance; }
};