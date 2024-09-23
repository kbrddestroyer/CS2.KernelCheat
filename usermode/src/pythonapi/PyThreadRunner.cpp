#include "PyThreadRunner.h"

void PyThreadRunner::Update()
{
	if (!isReady)
	{
		PythonInterpreter::createInterpreter(hDriver, uClient);

		isReady = true;
	}
	PythonInterpreter::Instance()->pymain();
}

void PyThreadRunner::createContext(HANDLE hDriver, uintptr_t uClient)
{
	ThreadedObject::createContext(hDriver, uClient);
}