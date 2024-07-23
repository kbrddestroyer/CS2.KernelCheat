#pragma once
#include <Python.h>
#include <Windows.h>
#include <string>

#include "static_config.h"

#pragma warning( disable : 4996) 

class WebserverEntry
{
private:
	bool activated = false;

private:
	PyObject* pCall(PyObject* pModule, const char* method, PyObject* args)
	{
		PyObject* pFunctionCall = PyObject_GetAttrString(pModule, method);
		if (!pFunctionCall || !PyCallable_Check(pFunctionCall))
			throw "Not callable";

		PyObject* pResult = PyObject_CallObject(pFunctionCall, args);

		Py_DECREF(pFunctionCall);
		return pResult;
	}

	void kernel()
	{
		Py_InitializeEx(0);	// Start python exec
		PyGILState_STATE gil = PyGILState_Ensure();

		if (!Py_IsInitialized())
		{
			MessageBoxA(NULL, "Failed to initialize python", "Error", MB_OK);
			return;
		}

		PyObject* pModule = PyImport_ImportModule("pythonscripts.webserver");

		if (pModule == nullptr)
		{
			PyErr_Print();
			MessageBoxA(NULL, "No module", "Error in Python", MB_OK);
			PyGILState_Release(gil);
			Py_FinalizeEx();
			return;
		}

		try {
			pCall(pModule, "init", nullptr);
		}
		catch (const char* e)
		{
			MessageBoxA(NULL, e, "Error", MB_OK);
			Py_DECREF(pModule);
			PyGILState_Release(gil);
			Py_FinalizeEx();
			return;
		}

		Py_XDECREF(pModule);
		PyGILState_Release(gil);
		
		if (Py_FinalizeEx() < 0)		// Finalize python exec
			MessageBox(NULL, L"Error while finalizing", L"Error!", MB_OK);
		MessageBox(NULL, L"Successfully authorized", L"OK!", MB_OK);
	}

public:
	void validate()
	{
		kernel();
	}
};

