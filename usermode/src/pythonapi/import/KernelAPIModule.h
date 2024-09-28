#pragma once
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <Windows.h>

#include "..\..\core\utils.h"

PyObject* kernelapi_init(PyObject*, PyObject*);
PyObject* kernelapi_read(PyObject*, PyObject*);
PyObject* kernelapi_write(PyObject*, PyObject*);
PyObject* kernelapi_uClient(PyObject*, PyObject*);

/* VARS */
inline static PyMethodDef kernelapiMethods[] = {
	{"init", kernelapi_init, METH_NOARGS, "Initialize kernelapi"},
	{"get_client", kernelapi_uClient, METH_NOARGS, "Get driver address as number from C"},
	{"read", kernelapi_read, METH_VARARGS | METH_KEYWORDS, "Read data from process memory"},
	{"write", kernelapi_write, METH_VARARGS | METH_KEYWORDS, "Write data to process memory"},
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
