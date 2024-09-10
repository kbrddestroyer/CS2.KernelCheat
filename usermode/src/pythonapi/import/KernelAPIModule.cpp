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
	
	if (!PyArg_ParseTuple(args, "l", &uAddress))
	{
		return Py_None;
	}

	uintptr_t uClient = PythonInterpreter::Instance()->Client();
	HANDLE hDriver = PythonInterpreter::Instance()->Driver();
	
	if (uAddress <= uClient)
	{
		return Py_None;
	}

	driver::read<void>(hDriver, uAddress);
	return Py_None;
}

PyObject* kernelapi_write(PyObject* self, PyObject* args)
{
	return Py_None;
}

PyObject* kernelapi_uClient(PyObject* self, PyObject* args)
{
	uintptr_t uClient = PythonInterpreter::Instance()->Client();
	return PyLong_FromInt64(uClient);
}

PyMODINIT_FUNC PyInit_kernelapi(void)
{
	return PyModule_Create(&kernelapimodule);
}