#include "KernelAPIModule.h"
#include "..\PythonAPI.h"


PyObject* kernelapi_init(PyObject* self, PyObject* args)
{
	MessageBoxA(NULL, "KernelAPI initialized", "OK!", MB_OK);
	
	return Py_None;
}

PyObject* kernelapi_read(PyObject* self, PyObject* args)
{
	uintptr_t uAddress;
	size_t uSize;
	if (!PyArg_ParseTuple(args, "KK", &uAddress, &uSize))
	{
		PyErr_SetString(PyExc_ValueError, "Invalid arguments: expected two integers (address, size).");
		return NULL;
	}

	uintptr_t uClient = PythonInterpreter::Instance()->Client();
	HANDLE hDriver = PythonInterpreter::Instance()->Driver();
	
	if (uAddress <= uClient)
	{
		Py_RETURN_NONE;
	}

	void* data = driver::readUnsafe(hDriver, uAddress, uSize);
	PyObject* returnValue = PyLong_FromVoidPtr(data);
	free(data);
	return returnValue;
}

PyObject* kernelapi_write(PyObject* self, PyObject* args)
{
	uintptr_t uAddress;
	size_t uSize;
	void* ptrData;
	if (!PyArg_ParseTuple(args, "KOK", &uAddress, &ptrData, &uSize))
	{
		PyErr_SetString(PyExc_ValueError, "Invalid arguments: expected two integers (address, size).");
		return NULL;
	}

	uintptr_t uClient = PythonInterpreter::Instance()->Client();
	HANDLE hDriver = PythonInterpreter::Instance()->Driver();

	if (uAddress <= uClient || uSize == 0 || !ptrData)
	{
		Py_RETURN_NONE;
	}

	driver::writeUnsafe(hDriver, uAddress, ptrData, uSize);
	Py_RETURN_NONE;
}

PyObject* kernelapi_uClient(PyObject* self, PyObject* args)
{
	uintptr_t uClient = PythonInterpreter::Instance()->Client();
	return PyLong_FromLongLong(uClient);
}

PyMODINIT_FUNC PyInit_kernelapi(void)
{
	return PyModule_Create(&kernelapimodule);
}