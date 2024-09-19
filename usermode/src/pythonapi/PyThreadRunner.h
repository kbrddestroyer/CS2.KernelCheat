#pragma once
#include "../core/cheat/ThreadController.h"
#include "PythonAPI.h"

class PyThreadRunner : public ThreadedObject
{
private:
	bool isReady = false;
public:
	void Update() override;

	void createContext(HANDLE, uintptr_t) override;
};

