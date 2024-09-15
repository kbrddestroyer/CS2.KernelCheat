#include "PyThreadRunner.h"

void PyThreadRunner::Update()
{

}

void PyThreadRunner::createContext(HANDLE hDriver, uintptr_t uClient)
{
	ThreadedObject::createContext(hDriver, uClient);
	PythonInterpreter::createInterpreter(hDriver, uClient);
}