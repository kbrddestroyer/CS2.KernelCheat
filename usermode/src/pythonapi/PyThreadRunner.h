#pragma once
#include "../core/cheat/ThreadController.h"
#include "PythonAPI.h"

class PyThreadRunner : public ThreadedObject
{
public:
	PyThreadRunner();

	void Update() override;
};

