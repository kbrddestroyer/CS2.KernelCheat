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

		if (!uPlayerPawn)
			continue;

		uintptr_t uGameSceneNode = driver::read<uintptr_t>(hDriver, uPlayerPawn + schemas::client_dll::C_BaseEntity::m_pGameSceneNode);

		if (!uGameSceneNode)
			continue;

		Vector3 uSpot = driver::read<Vector3>(hDriver, uGameSceneNode + schemas::client_dll::CGameSceneNode::m_vecAbsOrigin);
		Vector3 uRot = driver::read<Vector3>(hDriver, uGameSceneNode + schemas::client_dll::CGameSceneNode::m_angRotation);

		uint8_t uTeam = driver::read<uint8_t>(hDriver, uPlayerPawn + schemas::client_dll::C_BaseEntity::m_iTeamNum);

		CSEntity csEntity;
		csEntity.pos = uSpot;
		csEntity.uTeam = uTeam;
		csEntity.rotation = uRot.y;

		vEntities.push_back(csEntity);
	}

	bInitialised = true;
	std::this_thread::sleep_for(std::chrono::milliseconds(CFG_BHOP_DELAY));
}

ImVec2 RadarHack::gameToGUIPoint(Vector3 point, ImVec2 min, ImVec2 max)
{
	ImVec2 vSize = ImGui::GetWindowSize();
	ImVec2 vPos = ImGui::GetWindowPos();

	ImVec2 vGameScale = { min.x - max.x, min.y - max.y };
	ImVec2 vGamePos = { min.x - point.x, min.y - point.y };

	ImVec2 vScale = { vSize.x / vGameScale.x, vSize.y / vGameScale.y };

	return { vPos.x + vGamePos.x * vScale.x, vPos.y + vGamePos.y * vScale.y };
}

void RadarHack::Render(ImDrawList* imDrawList)
{
	ImGui::Checkbox("Enable debug", &this->bShowDebugInfo);

	for (CSEntity entity : vEntities)
	{
		if (bShowDebugInfo)
			ImGui::Text("%f %f %f", entity.pos.x, entity.pos.y, entity.pos.z);
		ImVec2 mapped = gameToGUIPoint(entity.pos, { -2500, -2500 }, { 2500, 2500 });
		ImVec2 mappedDir = { mapped.x + cos(entity.rotation * (3.14f / 180)) * 7, mapped.y + sin(entity.rotation * (3.14f / 180)) * 7 };
		ImColor imEntColor = (entity.uTeam == 2) ? ImColor(255, 0, 0) : ImColor(0, 0, 255);

		imDrawList->AddCircleFilled(gameToGUIPoint(entity.pos, { -MAX_MAX_COORD, -MAX_MAX_COORD }, { MAX_MAX_COORD, MAX_MAX_COORD }), 5, imEntColor);
		imDrawList->AddLine(mapped, mappedDir, imEntColor, 3);
	}
}
