#pragma once
#include "../core/cheat/ThreadController.h"
#include "PythonAPI.h"

class PyThreadRunner : public ThreadedObject
{
public:
	void Update() override;

	void createContext(HANDLE, uintptr_t) override;
};

