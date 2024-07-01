#include "Cheat.h"

void BhopCheat::Update(HANDLE hDriver, uintptr_t uClient)
{
#ifdef DEBUG
	if (GetAsyncKeyState(VK_ESCAPE))
		break;
#endif
	const uintptr_t playerPawn = driver::read<uintptr_t>(hDriver, uClient + offsets::client_dll::dwLocalPlayerPawn);

	if (playerPawn == 0)
		return;

	const uint32_t flags = driver::read<uint32_t>(hDriver, playerPawn + schemas::client_dll::C_BaseEntity::m_fFlags);
	const bool bInAir = flags & (1 << 0);

	const bool bSpace = GetAsyncKeyState(VK_SPACE);
	const DWORD forceJump = driver::read<DWORD>(hDriver, uClient + buttons::jump);

	if (bSpace && bInAir)
	{
		// Jump
		std::this_thread::sleep_for(std::chrono::milliseconds(CFG_BHOP_DELAY));
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

void BhopCheat::Render()
{

}

void RadarHack::Update(HANDLE hDriver, uintptr_t uClient)
{
	// Update thread

	uintptr_t uEntityList = driver::read<uintptr_t>(hDriver, uClient + offsets::client_dll::dwEntityList);
	vEntities.clear();
	for (uint32_t i = 0; i < 32; i++)
	{
		uintptr_t pEntity = driver::read<uintptr_t>(hDriver, uEntityList + i * 0x78);

		if (!pEntity)
			continue;
		
		vEntities.push_back(pEntity);
	}
	bInitialised = true;
	std::this_thread::sleep_for(std::chrono::milliseconds(CFG_BHOP_DELAY));
}

void RadarHack::Render()
{

}
