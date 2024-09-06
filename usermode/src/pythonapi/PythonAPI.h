#pragma once
#include <Python.h>
#include <Windows.h>

#include <filesystem>

class PythonAPI
{
public:
	inline static void entrypoint()
	{
		std::string wHome = std::filesystem::current_path().string() + "\\LibPortable";
		Py_SetPythonHome((const wchar_t*) wHome.c_str());
		Py_InitializeEx(0);

		Py_FinalizeEx();
	}
};
