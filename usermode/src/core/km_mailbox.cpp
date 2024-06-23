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
		std::thread tBhop = std::thread(threadedBhop, std::ref(hDriver), std::ref(uClient));

		tBhop.join();
	}
}

void threadedBhop(HANDLE& hDriver, const uintptr_t& uClient)
{
#ifdef DEBUG
	MessageBox(NULL, L"Mapped client.dll", L"Info", MB_ICONINFORMATION | MB_OK);
#endif

	while (true)
	{
#ifdef DEBUG
		if (GetAsyncKeyState(VK_ESCAPE))
			break;
#endif
		const uintptr_t playerPawn = driver::read<uintptr_t>(hDriver, uClient + offsets::client_dll::dwLocalPlayerPawn);

		if (playerPawn == 0)
			continue;

		const uint32_t flags = driver::read<uint32_t>(hDriver, playerPawn + schemas::client_dll::C_BaseEntity::m_fFlags);
		const bool bInAir = flags & (1 << 0);

		const bool bSpace = GetAsyncKeyState(VK_SPACE);
		const DWORD forceJump = driver::read<DWORD>(hDriver, uClient + buttons::jump);

		if (bSpace && bInAir)
		{
			// Jump
			Sleep(CFG_BHOP_DELAY);
			driver::write(hDriver, uClient + buttons::jump, CFG_SPACE_ON);
		}
		else if (
			(bSpace && !bInAir) ||
			(!bSpace && forceJump == 65537)
			)
		{
			driver::write(hDriver, uClient + buttons::jump, CFG_SPACE_OFF);
		}
	}
}