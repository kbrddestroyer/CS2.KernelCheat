#include "Cheat.h"
#include "../gui/GUIController.h"

using namespace cheatscore::core;

void Cheat::Update(HANDLE hDriver, uintptr_t uClient)
{
	if (bState.load(std::memory_order_relaxed))
		CheatUpdate(hDriver, uClient);
}

void BhopCheat::CheatUpdate(HANDLE hDriver, uintptr_t uClient)
{
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
		driver::write(hDriver, uClient + buttons::jump, MEM_PRESSED);
		driver::write(hDriver, uClient + buttons::duck, MEM_PRESSED);
	}
	else if (
		(bSpace && !bInAir) ||
		(!bSpace && forceJump == 65537)
		)
	{
		driver::write(hDriver, uClient + buttons::jump, MEM_RELEASED);
		driver::write(hDriver, uClient + buttons::duck, MEM_RELEASED);
	}
}

void BhopCheat::Render()
{

}

void RadarHack::CheatUpdate(HANDLE hDriver, uintptr_t uClient)
{
	if (!this->bState.load())
		return;
	if (isBusyRendering.load())
		return;

	vEntities.clear();

	uintptr_t uEntityList = driver::read<uintptr_t>(hDriver, uClient + offsets::client_dll::dwEntityList);

	if (!uEntityList)
		return;

	uintptr_t pLocalPlayer = driver::read<uintptr_t>(hDriver, uClient + offsets::client_dll::dwLocalPlayerPawn);

	if (!pLocalPlayer)
		return;

	uintptr_t uLocalGameSceneNode = driver::read<uintptr_t>(hDriver, pLocalPlayer + schemas::client_dll::C_BaseEntity::m_pGameSceneNode);

	if (!uLocalGameSceneNode)
		return;

	Vector3f uLocalSpot = driver::read<Vector3f>(hDriver, uLocalGameSceneNode + schemas::client_dll::CGameSceneNode::m_vecAbsOrigin);
	Vector3f vLocalOldSpot = driver::read<Vector3f>(hDriver, pLocalPlayer + schemas::client_dll::C_BasePlayerPawn::m_vOldOrigin);
	Vector3f vLocalHeadOffset = driver::read<Vector3f>(hDriver, pLocalPlayer + schemas::client_dll::C_BaseModelEntity::m_vecViewOffset);
	Vector3f vLocalHeadPosition = vLocalOldSpot + vLocalHeadOffset;
	QAngle	 uLocalRot = driver::read<QAngle>(hDriver, uLocalGameSceneNode + schemas::client_dll::CGameSceneNode::m_angRotation);

	uint8_t  uLocalTeam = driver::read<uint8_t>(hDriver, pLocalPlayer + schemas::client_dll::C_BaseEntity::m_iTeamNum);
	uint32_t uLocalHealth = driver::read<uint32_t>(hDriver, pLocalPlayer + schemas::client_dll::C_BaseEntity::m_iHealth);
	uint32_t uLocalArmor = driver::read<uint32_t>(hDriver, pLocalPlayer + schemas::client_dll::C_CSPlayerPawn::m_ArmorValue);

	localEntity = CSPlayerEntity(pLocalPlayer, "Local Player", uLocalTeam, uLocalHealth, uLocalArmor, uLocalSpot, vLocalOldSpot, vLocalHeadPosition, uLocalRot, true, true);

	for (uint32_t i = 0; i < 32; i++)
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
		{
			localEntity.uIndex = i;
			continue;
		}

		uintptr_t uGameSceneNode = driver::read<uintptr_t>(hDriver, uPlayerPawn + schemas::client_dll::C_BaseEntity::m_pGameSceneNode);

		if (!uGameSceneNode)
			continue;

		int32_t		uHealth = driver::read<int32_t>(hDriver, uPlayerPawn + schemas::client_dll::C_BaseEntity::m_iHealth);
		
		if (uHealth <= 0)
			continue;
		
		Vector3f	vSpot = driver::read<Vector3f>(hDriver, uGameSceneNode + schemas::client_dll::CGameSceneNode::m_vecAbsOrigin);
		Vector3f	vOldOrigin = driver::read<Vector3f>(hDriver, uPlayerPawn + schemas::client_dll::C_BasePlayerPawn::m_vOldOrigin);
		QAngle		vRot = driver::read<QAngle>(hDriver, uGameSceneNode + schemas::client_dll::CGameSceneNode::m_angRotation);
		Vector3f    vHeadOffset = driver::read<Vector3f>(hDriver, uPlayerPawn + schemas::client_dll::C_BaseModelEntity::m_vecViewOffset);
		Vector3f	vHeadPosition = vOldOrigin + vHeadOffset;
		uint8_t		uTeam = driver::read<uint8_t>(hDriver, uPlayerPawn + schemas::client_dll::C_BaseEntity::m_iTeamNum);
		if (uTeam == 0)
			continue;
		
		uint32_t	uArmor = driver::read<uint32_t>(hDriver, uPlayerPawn + schemas::client_dll::C_CSPlayerPawn::m_ArmorValue);
		uint32_t	isSpotted = driver::read<uint32_t>(hDriver, uPlayerPawn + schemas::client_dll::C_CSPlayerPawn::m_entitySpottedState + schemas::client_dll::EntitySpottedState_t::m_bSpottedByMask);

		CSPlayerEntity csEntity(i, "Player", uTeam, uHealth, uArmor, vSpot, vOldOrigin, vHeadPosition, vRot, isSpotted);

		vEntities.push_back(csEntity);
	}
}

void RadarHack::Render()
{
	if (!bState.load())
		return;

	ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowBgAlpha(0.0f);
	ImGui::Begin("Radar", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);

	ImGui::Checkbox("Enable debug", &this->bShowDebugInfo);

	ImVec2 vSize = ImGui::GetWindowSize();
	ImVec2 vPosition = ImGui::GetWindowPos();

	ImVec2 position = { vPosition.x + vSize.x / 2, vPosition.y + vSize.y / 2 };

	ImDrawList* imDrawList = ImGui::GetWindowDrawList();

	isBusyRendering.store(true);

	localEntity.Render(imDrawList, position, -90);

	if (bShowDebugInfo)
	{
		ImGui::Text("%d", localEntity.isSpotted);
	}

	imDrawList->AddLine({ vPosition.x, vPosition.y + vSize.y / 2 }, { vPosition.x + vSize.x, vPosition.y + vSize.y / 2 }, ImColor(255, 255, 255, 100));
	imDrawList->AddLine({ vPosition.x + vSize.x / 2, vPosition.y }, { vPosition.x + vSize.x / 2, vPosition.y + vSize.y }, ImColor(255, 255, 255, 100));

	for (CSPlayerEntity entity : vEntities)
	{
		if (entity.uHealth == 0)
			continue;

		if (bShowDebugInfo)
		{
			ImGui::Text("%d", entity.isSpotted);
		}
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
		entity.Render(imDrawList, vGUIRotated, localEntity.qAngle.y - entity.qAngle.y - 90);
	}

	isBusyRendering.store(false);
	ImGui::End();
}

void TriggerBot::CheatUpdate(HANDLE hDriver, uintptr_t uClient)
{
	uintptr_t uLocalPlayer = driver::read<uintptr_t>(hDriver, uClient + offsets::client_dll::dwLocalPlayerPawn);
	uint8_t uLocalTeam = driver::read<uint8_t>(hDriver, uLocalPlayer + schemas::client_dll::C_BaseEntity::m_iTeamNum);

	uintptr_t uEntityList = driver::read<uintptr_t>(hDriver, uClient + offsets::client_dll::dwEntityList);

	if (!uLocalPlayer || !uEntityList)
		return;

	int32_t iCrosshairEntityID = driver::read<int32_t>(hDriver, uLocalPlayer + schemas::client_dll::C_CSPlayerPawnBase::m_iIDEntIndex);
	if (iCrosshairEntityID < 0)
		return;
	uintptr_t pEntityListEntry = driver::read<uintptr_t>(hDriver, uEntityList + 8 * (iCrosshairEntityID >> 9) + 0x10);
	if (!pEntityListEntry)
		return;
	uintptr_t uPlayerPawn = driver::read<uintptr_t>(hDriver, pEntityListEntry + 120 * (iCrosshairEntityID & 0x1FF));

	if (!uPlayerPawn)
		return;

	int8_t uTeam = driver::read<uint8_t>(hDriver, uPlayerPawn + schemas::client_dll::C_BaseEntity::m_iTeamNum);
	int32_t iHealth = driver::read<int32_t>(hDriver, uPlayerPawn + schemas::client_dll::C_BaseEntity::m_iHealth);

	if (uTeam == uLocalTeam || uTeam == 0 || iHealth < 0)
		return;

	driver::write(hDriver, uClient + buttons::attack, MEM_PRESSED);
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	driver::write(hDriver, uClient + buttons::attack, MEM_RELEASED);
	std::this_thread::sleep_for(std::chrono::milliseconds(SettingsTab::getInstance()->triggerDelay));
}

void TriggerBot::Render()
{

}

void AimBot::CheatUpdate(HANDLE hDriver, uintptr_t uClient)
{
	uintptr_t uEntityList = driver::read<uintptr_t>(hDriver, uClient + offsets::client_dll::dwEntityList);

	if (!uEntityList)
		return;

	uintptr_t pLocalPlayer = driver::read<uintptr_t>(hDriver, uClient + offsets::client_dll::dwLocalPlayerPawn);

	if (!pLocalPlayer)
		return;

	uintptr_t uLocalGameSceneNode = driver::read<uintptr_t>(hDriver, pLocalPlayer + schemas::client_dll::C_BaseEntity::m_pGameSceneNode);

	if (!uLocalGameSceneNode)
		return;

	uint32_t uLocalHealth = driver::read<uint32_t>(hDriver, pLocalPlayer + schemas::client_dll::C_BaseEntity::m_iHealth);
	if (uLocalHealth == 0)
		return;


	Vector3f vLocalOldSpot = driver::read<Vector3f>(hDriver, pLocalPlayer + schemas::client_dll::C_BasePlayerPawn::m_vOldOrigin);
	Vector3f vLocalHeadOffset = driver::read<Vector3f>(hDriver, pLocalPlayer + schemas::client_dll::C_BaseModelEntity::m_vecViewOffset);
	Vector3f vLocalHeadPosition = vLocalOldSpot + vLocalHeadOffset;

	uint8_t  uLocalTeam = driver::read<uint8_t>(hDriver, pLocalPlayer + schemas::client_dll::C_BaseEntity::m_iTeamNum);
	uint8_t	 uLocalIndex = 0;
	Vector3f vClosestHeadPosition;
	uint32_t uClosestSpotted;
	float fClosestDistance = -1;

	for (uint32_t i = 0; i < 32; i++)
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
		{
			uLocalIndex = i;
			continue;
		}

		uintptr_t uGameSceneNode = driver::read<uintptr_t>(hDriver, uPlayerPawn + schemas::client_dll::C_BaseEntity::m_pGameSceneNode);

		if (!uGameSceneNode)
			continue;

		int32_t uHealth = driver::read<int32_t>(hDriver, uPlayerPawn + schemas::client_dll::C_BaseEntity::m_iHealth);
		if (uHealth <= 0 || uHealth > 100)
			continue;
		uint8_t		uTeam = driver::read<uint8_t>(hDriver, uPlayerPawn + schemas::client_dll::C_BaseEntity::m_iTeamNum);
		if (uTeam == uLocalTeam)
			continue;

		Vector3f	vOldOrigin = driver::read<Vector3f>(hDriver, uPlayerPawn + schemas::client_dll::C_BasePlayerPawn::m_vOldOrigin);
		Vector3f    vHeadOffset = driver::read<Vector3f>(hDriver, uPlayerPawn + schemas::client_dll::C_BaseModelEntity::m_vecViewOffset);
		Vector3f	vHeadPosition = vOldOrigin + vHeadOffset;
		uint32_t	isSpotted = driver::read<uint32_t>(hDriver, uPlayerPawn + schemas::client_dll::C_CSPlayerPawn::m_entitySpottedState + schemas::client_dll::EntitySpottedState_t::m_bSpottedByMask);

		float fDistance = distance(vLocalHeadPosition, vHeadPosition);
		if (fClosestDistance < 0 || fDistance < fClosestDistance)
		{
			fClosestDistance = fDistance;
			vClosestHeadPosition = vHeadPosition;
			uClosestSpotted = isSpotted;
		}
	}

	if (fClosestDistance < 0)
		return;
	if (!(uClosestSpotted & (1 << uLocalIndex - 1)))
		return;

	float fDistance = fClosestDistance;

	Vector3f vOffset = vClosestHeadPosition - vLocalHeadPosition;
	QAngle angle = toAngle(vClosestHeadPosition - vLocalHeadPosition);

	QAngle oldAngle = driver::read<QAngle>(hDriver, uClient + offsets::client_dll::dwViewAngles);
	QAngle delta = oldAngle - angle;

	if (magnitude(delta) > SettingsTab::getInstance()->aimbotMaxDistance / 2)
		return;

	QAngle newAngle = oldAngle - delta / SettingsTab::getInstance()->aimbotSmoothness;

	driver::write<QAngle>(hDriver, uClient + offsets::client_dll::dwViewAngles, angle);
}

void AimBot::Render() {
	ImDrawList* imBackground = ImGui::GetBackgroundDrawList();

	ImVec2 vSize = ImGui::GetIO().DisplaySize;
	imBackground->AddCircle({ vSize.x / 2, vSize.y / 2 }, SettingsTab::getInstance()->aimbotMaxDistance * 5, ImColor(255, 255, 255, 255), 0, 2);
}