#include "PythonAPI.h"


void PythonAPIException::showErrorMessage()
{
	MessageBoxA(NULL, this->error.c_str(), "Python API error", MB_OK | MB_ICONERROR);
}

void PythonInterpreter::createInterpreter(HANDLE hDriver, uintptr_t uClient)
{
	try
	{
		if (PythonInterpreter::pyGlobalPointer)
			throw PythonAPIException("Collision: Cannot create multiple instances of python interpreter.");
		PythonInterpreter::pyGlobalPointer = std::make_unique<PythonInterpreter>(hDriver, uClient);
		PythonInterpreter::Instance()->postinit();
	}
	catch (PythonAPIException e)
	{
		e.showErrorMessage();
	}
}

std::string PythonInterpreter::fetchPath() const
{
	// 1st attempt to fetch python libs from working directory

	std::string pythonpath = std::filesystem::current_path().string();
	
	if (std::filesystem::exists(pythonpath + LIB_FOLDER))
		return pythonpath;
	// 2nd attempt: assume working from vs debugger, so find build directory and search for LibPortable folder
	
	pythonpath = std::filesystem::current_path().parent_path().string() + "\\x64\\build";
	if (std::filesystem::exists(pythonpath + LIB_FOLDER))
		return pythonpath;

	// Error
	throw PythonAPIException("Cannot find LibPortable folder in any build destinations. Python won't be enabled");
}

/**
* Python kernelmode API initialization point
* 
* @throws PythonAPIException(std::string)
*/
bool PythonInterpreter::pymain()
{
	if (!Py_IsInitialized() || !bInitialied)
		return false;
	if (!entry)
		return false;
	pCallSafe(entry, "update");
	return true;
}

bool PythonInterpreter::initialize() noexcept
{
	try
	{
		pythonpath = fetchPath();
	}
	catch (PythonAPIException e)
	{
		e.showErrorMessage();
		return false;
	}

	pythonpath += LIB_FOLDER;

	this->initializeModules();

	Py_InitializeEx(0);

	PyObject* sysPath = PySys_GetObject("path");
	PyObject* currentPath = PyUnicode_FromString(pythonpath.c_str());
	PyList_Append(sysPath, currentPath);
	Py_XDECREF(currentPath);

	this->gil = PyGILState_Ensure();

	PyEval_InitThreads();

	if (!Py_IsInitialized())
	{
		throw PythonAPIException("Python was not initialized");
		return false;
	}
	
	PyObject* sys_path = PySys_GetObject("path");
	PyObject* folder_path = PyUnicode_FromString("F:\\Work\\CS2.KernelCheat\\x64\\build\\kernel");

	PyList_Append(sys_path, folder_path);

	PyObject* pModuleName = PyUnicode_FromString(PY_ENTRY_MODULE);
	entry = PyImport_Import(pModuleName);
	Py_XDECREF(pModuleName);
	
	if (!entry)
	{
		MessageBoxA(NULL, "Error: No entry module found! Python could not be started", "Python internal error", MB_OK);

		finalize();
		return false;
	}
}

void PythonInterpreter::postinit() noexcept
{
	try
	{
		PyObject* args = Py_BuildValue("(s)", pythonpath.c_str());
		if (!args)
		{
			throw PythonAPIException("Internal error");
		}
		pCallSafe(entry, "invoke", args);
	}
	catch (PythonAPIException e)
	{
		e.showErrorMessage();
	}
}

void PythonInterpreter::finalize() noexcept
{
	if (!Py_IsInitialized())
		return;
	if (entry)
	{
		pCallSafe(entry, "destroy");
		Py_XDECREF(entry);
	}
	PyGILState_Release(gil);

	if (Py_FinalizeEx() < 0)
	{
		throw PythonAPIException("Critical! PythonAPI could not finalize");
	}
	bInitialied = false;
}

PyObject* PythonInterpreter::pCall(PyObject* pModule, const char* method, PyObject* args)
{
	if (!Py_IsInitialized())
	{
		throw PythonAPIException("An attempt of function call with no python initialized");
	}
	if (!pModule)
	{
		throw PythonAPIException("pModule is null");
	}
	PyObject* pFunctionCall = PyObject_GetAttrString(pModule, method);
	if (!pFunctionCall || !PyCallable_Check(pFunctionCall))
	{
		Py_XDECREF(pFunctionCall);
		throw PythonAPIException("Not callable invoke");
	}
	PyObject* pResult = PyObject_CallObject(pFunctionCall, args);

	Py_DECREF(pFunctionCall);
	return pResult;
}

bool PythonInterpreter::initializeModules()
{
	if (PyImport_AppendInittab("kernelapi", PyInit_kernelapi) == -1) {
		throw PythonAPIException("Cannot import built-in module kernelapi");
	}
}

PyObject* PythonInterpreter::pCallSafe(PyObject* pModule, const char* method, PyObject* args)
{
	try
	{
		return pCall(pModule, method, args);
	}
	catch (PythonAPIException e)
	{
		e.showErrorMessage();
		return nullptr;
	}
}
