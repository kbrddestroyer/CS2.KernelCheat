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

void BhopCheat::Render(ImDrawList*)
{

}

void RadarHack::Update(HANDLE hDriver, uintptr_t uClient)
{
	// Update thread

	uintptr_t uEntityList = driver::read<uintptr_t>(hDriver, uClient + offsets::client_dll::dwEntityList);

	if (!uEntityList)
		return;
	vEntities.clear();

	uintptr_t pLocalPlayer = driver::read<uintptr_t>(hDriver, uClient + offsets::client_dll::dwLocalPlayerPawn);

	if (!pLocalPlayer)
		return;

	uintptr_t uLocalGameSceneNode = driver::read<uintptr_t>(hDriver, pLocalPlayer + schemas::client_dll::C_BaseEntity::m_pGameSceneNode);

	if (!uLocalGameSceneNode)
		return;

	Vector3f uLocalSpot = driver::read<Vector3f>(hDriver, uLocalGameSceneNode + schemas::client_dll::CGameSceneNode::m_vecAbsOrigin);
	QAngle uLocalRot = driver::read<QAngle>(hDriver, uLocalGameSceneNode + schemas::client_dll::CGameSceneNode::m_angRotation);

	uint8_t uLocalTeam = driver::read<uint8_t>(hDriver, pLocalPlayer + schemas::client_dll::C_BaseEntity::m_iTeamNum);
	uint32_t uLocalHealth = driver::read<uint32_t>(hDriver, pLocalPlayer + schemas::client_dll::C_BaseEntity::m_iHealth);
	uint32_t uLocalArmor = driver::read<uint32_t>(hDriver, pLocalPlayer + schemas::client_dll::C_CSPlayerPawn::m_ArmorValue);

	localEntity = RadarEntity("Local Player", uLocalTeam, uLocalHealth, uLocalArmor, uLocalSpot, uLocalRot);

	for (uint32_t i = 0; i < 64; i++)
	{
		uintptr_t pEntityListEntry = driver::read<uintptr_t>(hDriver, uEntityList + (8 * (i & 0x7FF) >> 9) + 16);
		if (!pEntityListEntry)
			continue;
		uintptr_t uPlayerController = driver::read<uintptr_t>(hDriver, pEntityListEntry + 120 * (i & 0x1FF));
		if (!uPlayerController)
			continue;

		uint32_t uPlayerPawnIndex = driver::read<uint32_t>(hDriver, uPlayerController + schemas::client_dll::CCSPlayerController::m_hPlayerPawn);
		if (!uPlayerPawnIndex)
			continue;
		uintptr_t uPlayerPawn = driver::read<uintptr_t>(hDriver, pEntityListEntry + 120 * (uPlayerPawnIndex & 0x1FF));

		if (!uPlayerPawn || uPlayerPawn == pLocalPlayer)
			continue;

		uintptr_t uGameSceneNode = driver::read<uintptr_t>(hDriver, uPlayerPawn + schemas::client_dll::C_BaseEntity::m_pGameSceneNode);

		if (!uGameSceneNode)
			continue;

		Vector3f uSpot = driver::read<Vector3f>(hDriver, uGameSceneNode + schemas::client_dll::CGameSceneNode::m_vecAbsOrigin);
		QAngle uRot = driver::read<QAngle>(hDriver, uGameSceneNode + schemas::client_dll::CGameSceneNode::m_angRotation);

		uint8_t uTeam = driver::read<uint8_t>(hDriver, uPlayerPawn + schemas::client_dll::C_BaseEntity::m_iTeamNum);
		uint32_t uHealth = driver::read<uint32_t>(hDriver, uPlayerPawn + schemas::client_dll::C_BaseEntity::m_iHealth);
		uint32_t uArmor = driver::read<uint32_t>(hDriver, uPlayerPawn + schemas::client_dll::C_CSPlayerPawn::m_ArmorValue);
		
		RadarEntity csEntity("Player", uTeam, uHealth, uArmor, uSpot, uRot);
		vEntities.push_back(csEntity);
	}

	bInitialised = true;
	std::this_thread::sleep_for(std::chrono::milliseconds(CFG_BHOP_DELAY));
}

void RadarHack::Render(ImDrawList* imDrawList)
{
	ThreadMgr::getInstance()->getMutex().lock();
	ImGui::Checkbox("Enable debug", &this->bShowDebugInfo);

	ImVec2 vSize = ImGui::GetWindowSize();
	ImVec2 vPosition = ImGui::GetWindowPos();

	ImVec2 position = { vPosition.x + vSize.x / 2, vPosition.y + vSize.y / 2 };

	localEntity.Render(imDrawList, position, -90);

	if (bShowDebugInfo)
	{
		ImGui::Text("%f", localEntity.qAngle.y);
	}

	imDrawList->AddLine({ vPosition.x, vPosition.y + vSize.y / 2 }, { vPosition.x + vSize.x, vPosition.y + vSize.y / 2 }, ImColor(255, 255, 255, 100));
	imDrawList->AddLine({ vPosition.x + vSize.x / 2, vPosition.y }, { vPosition.x + vSize.x / 2, vPosition.y + vSize.y }, ImColor(255, 255, 255, 100));

	for (RadarEntity& entity : vEntities)
	{
		// 1. Count in-game distance
		Vector3f vDistance = entity.vPosition - localEntity.vPosition;
		vDistance.x *= -1;
		int multiplier = vDistance.x * vDistance.y;
		multiplier /= fabs(multiplier);
		// 2. Count angles
		float fAngleBetween = acos((entity.vPosition.x * localEntity.vPosition.x + entity.vPosition.y * localEntity.vPosition.y) / (magnitude(entity.vPosition) * magnitude(localEntity.vPosition)));
		float fGUIAngle = (((localEntity.qAngle.y - fAngleBetween)) + 90) * 3.14 / 180;

		Vector3f vRotated = rotate(vDistance, fGUIAngle);
		ImVec2 vGUIRotated = gameToGUIPoint(vRotated);
		entity.Render(imDrawList, vGUIRotated, localEntity.qAngle.y + entity.qAngle.y - fAngleBetween + 90);
	}

	ThreadMgr::getInstance()->getMutex().unlock();
}
