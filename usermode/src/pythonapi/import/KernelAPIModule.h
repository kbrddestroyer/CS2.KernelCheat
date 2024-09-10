#pragma once
#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <Windows.h>


PyObject* kernelapi_init(PyObject*, PyObject*);

/* VARS */
inline static PyMethodDef kernelapiMethods[] = {
	{"init", kernelapi_init, METH_NOARGS, "Initialize kernelapi"},
	{NULL, NULL, 0, NULL}
};

inline static PyModuleDef kernelapimodule = {
	PyModuleDef_HEAD_INIT,
	"kernelapi",
	NULL,
	-1,
	kernelapiMethods
};

PyMODINIT_FUNC PyInit_kernelapi(void);
