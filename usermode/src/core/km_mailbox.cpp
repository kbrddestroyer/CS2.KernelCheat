#include "km_mailbox.h"
#include "../pythonapi/PythonAPI.h"

int kmControllerEntry()
{
#ifndef GUI_DEBUG_MODE
	const DWORD dPid = uapp::getPID(TARGET_PNAME);

	if (dPid == 0)
	{
		MessageBox(NULL, L"Please, open target process first! [cs2.exe]", L"Warning", MB_OK);
		return EXIT_FAILURE;
	}

	HANDLE hDriverHandle = CreateFile(L"\\\\.\\KM_KBRDCS2RWAPP", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (hDriverHandle == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, L"Invalid handle", L"Error", MB_ICONERROR | MB_OK);
		return EXIT_FAILURE;
	}

	if (driver::attach(hDriverHandle, dPid) == true)
	{
		initialize(hDriverHandle, dPid);
	}

	if (hDriverHandle != nullptr)
		CloseHandle(hDriverHandle);

	return EXIT_SUCCESS;
#else
	MessageBoxA(NULL, "Warning!", "The cheat will run in GUI debug mode", MB_OK);
	return EXIT_SUCCESS;
#endif
}

void initialize(HANDLE hDriver, DWORD uPid)
{
	if (const uintptr_t uClient = uapp::getModuleBase(uPid, L"client.dll"); uClient != 0)
	{
		ThreadMgr* thread = ThreadMgr::getInstance();
		thread->setKMParams(hDriver, uClient);
	}
}
