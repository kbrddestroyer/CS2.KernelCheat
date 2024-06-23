#define DIRECT3D_11

#include <iostream>
#include <Windows.h>

#include "utils.h"
#include "constants.h"

int kmControllerEntry()
{
	std::cout << "Starting mapper" << std::endl;
	std::cout << "BASIC SINGATURE INFORMATION" << std::endl;
	std::cout << "DATE: " << __TIMESTAMP__ << std::endl;
	const DWORD dPid = uapp::getPID(TARGET_PNAME);

	if (dPid == 0)
	{
		std::cout << "Please, run target app first [notepad.exe]" << std::endl;
		system("pause");
		return EXIT_FAILURE;
	}

	HANDLE hDriverHandle = CreateFile(L"\\\\.\\KM_KBRDCS2RWAPP", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (hDriverHandle == INVALID_HANDLE_VALUE)
	{
		std::cout << "INVALID" << std::endl;
		system("pause");
		return EXIT_FAILURE;
	}

	if (driver::attach(hDriverHandle, dPid) == true)
	{
		std::cout << "Attachement success!" << std::endl;
		system("pause");
	}

	std::cout << "Assuming, that driver was successfully attached to a target process" << std::endl;
	std::cout << "TARGET PID: " << dPid << std::endl;
	std::cout << "TARGET:     " << (char*) TARGET_PNAME << std::endl;
	std::cout << "***" << std::endl;


	if (hDriverHandle != nullptr)
		CloseHandle(hDriverHandle);

	system("pause");
	return EXIT_SUCCESS;
}