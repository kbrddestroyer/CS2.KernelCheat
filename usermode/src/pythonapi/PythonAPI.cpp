#include "PythonAPI.h"


void PythonAPIException::showErrorMessage()
{
	MessageBoxA(NULL, this->error.c_str(), "Python API error", MB_OK | MB_ICONERROR);
}

void PythonInterpreter::createInterpreter()
{
	// Try to initialize, handle error

	try
	{
		if (pyGlobalPointer)
			throw PythonAPIException("Collision: Cannot create multiple instances of python interpreter.");
		pyGlobalPointer = std::make_shared<PythonInterpreter>();
	}
	catch (PythonAPIException e)
	{
		e.showErrorMessage();
	}
}

std::wstring PythonInterpreter::fetchPath() const
{
	// 1st attempt to fetch python libs from working directory

	std::wstring pythonpath = std::filesystem::current_path().wstring() + LIB_FOLDER;
	
	if (std::filesystem::exists(pythonpath))
		return pythonpath;
	// 2nd attempt: assume working from vs debugger, so find build directory and search for LibPortable folder
	
	pythonpath = std::filesystem::current_path().parent_path().wstring() + L"\\x64\\build" + LIB_FOLDER;
	if (std::filesystem::exists(pythonpath))
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
	return true;
}

bool PythonInterpreter::initialize() noexcept
{
	std::wstring pythonpath;
	try
	{
		pythonpath = fetchPath();
	}
	catch (PythonAPIException e)
	{
		e.showErrorMessage();
		return false;
	}
	Py_SetPythonHome(pythonpath.c_str());

	Py_InitializeEx(0);
	this->gil = PyGILState_Ensure();
}

void PythonInterpreter::finalize() noexcept
{
	PyGILState_Release(gil);
	if (!Py_FinalizeEx())
	{
		throw PythonAPIException("Critical! PythonAPI could not finalize");
	}
}
