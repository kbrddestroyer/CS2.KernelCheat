#include "KernelAPIModule.h"

PyObject* kernelapi_init(PyObject* self, PyObject* args)
{
	MessageBoxA(NULL, "KernelAPI initialized", "OK!", MB_OK);
	return Py_None;
}

PyMODINIT_FUNC PyInit_kernelapi(void)
{
	return PyModule_Create(&kernelapimodule);
}