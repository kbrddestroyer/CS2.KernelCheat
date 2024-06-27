#include "km_mailbox.h"

int kmControllerEntry()
{
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
		MessageBox(NULL, L"Succecssfully attached kernelmode to target", L"Success", MB_ICONINFORMATION | MB_OK);

		initialize(hDriverHandle, dPid);
	}

	if (hDriverHandle != nullptr)
		CloseHandle(hDriverHandle);

	return EXIT_SUCCESS;
}

void initialize(HANDLE hDriver, DWORD uPid)
{
	if (const uintptr_t uClient = uapp::getModuleBase(uPid, L"client.dll"); uClient != 0)
	{
#ifdef DEBUG
		MessageBox(NULL, L"Mapped client.dll", L"Info", MB_ICONINFORMATION | MB_OK);
#endif
		ThreadMgr* thread = ThreadMgr::getInstance();

		BhopCheat* cheat = new BhopCheat();
		thread->getThread()->addElement(cheat);
	}
}
