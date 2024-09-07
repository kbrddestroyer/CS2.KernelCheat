#pragma once
#include <Python.h>
#include <Windows.h>

#include <filesystem>

#include "../core/cheat/ThreadController.h"

#ifndef LIB_FOLDER
#define LIB_FOLDER L"\\LibPortable\\"
#endif

#ifndef PY_DELAY
#define PY_DELAY 1
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
	std::wstring fetchPath() const;
	PyGILState_STATE gil;
	PyObject* entry;
	bool bInitialied = false;
public:
	static std::weak_ptr<PythonInterpreter> Instance() { return std::weak_ptr(pyGlobalPointer).lock(); }
	static void createInterpreter();
public:
	PythonInterpreter()
	{
		bInitialied = initialize();
		pymain();
	}

	~PythonInterpreter()
	{
		pyGlobalPointer = nullptr;
		if (bInitialied)
			finalize();
	}
private:
	PyObject* pCall(PyObject* pModule, const char* method, PyObject* args = nullptr)
	{
		PyObject* pFunctionCall = PyObject_GetAttrString(pModule, method);
		if (!pFunctionCall || !PyCallable_Check(pFunctionCall))
			throw PythonAPIException("Not callable invoke");

		PyObject* pResult = PyObject_CallObject(pFunctionCall, args);

		Py_DECREF(pFunctionCall);
		return pResult;
	}

	bool initialize() noexcept;
	bool pymain();
	void finalize() noexcept;
};
