#pragma once
#include <Python.h>

#include <Windows.h>
#include <filesystem>

#include "../core/cheat/ThreadController.h"
#include "import/KernelAPIModule.h"

#ifndef LIB_FOLDER
#define LIB_FOLDER "\\kernelapi"
#endif

#ifndef PY_DELAY
#define PY_DELAY 1
#endif

#ifndef PY_ENTRY_MODULE
#define PY_ENTRY_MODULE "pyext_api"
#endif

#ifndef PY_ENTRY_FUNCTION
#define PY_ENTRY_FUNCTION "invoke"
#endif

/**
* Python kernelmodeAPI exception
*/
class PythonAPIException
{
private:
	std::string error;
public:
	PythonAPIException(std::string error = "")
	{
		this->error = error;
	}

	std::string e() { return error; }

	// Displays error message as MessageBoxA
	void showErrorMessage();
};

/**
* KernelAPI entrypoint, configuration, tools, etc.
* handles python runtime
*/
class PythonInterpreter
{
private:
	inline static std::shared_ptr<PythonInterpreter> pyGlobalPointer;
private:
	std::string fetchPath() const;
	PyGILState_STATE gil;
	PyObject* entry;
	bool bInitialied = false;
	bool bMainRunning = false;
public:
	static std::weak_ptr<PythonInterpreter> Instance() { return std::weak_ptr(pyGlobalPointer).lock(); }
	static void createInterpreter();
public:
	PythonInterpreter()
	{
		bInitialied = initialize();
		if (!pymain())
			throw PythonAPIException("Cannot run main");
	}

	~PythonInterpreter()
	{
		pyGlobalPointer = nullptr;
		if (bInitialied)
			finalize();
	}
private:
	PyObject* pCall(PyObject* pModule, const char* method, PyObject* args = nullptr);
	bool initialize() noexcept;
	bool pymain();
	void finalize() noexcept;
	
	bool initializeModules();
public:
	PyObject* pCallSafe(PyObject* pModule, const char* method, PyObject* args = nullptr);
};
