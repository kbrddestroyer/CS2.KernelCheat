#include "Cheat.h"
#include "../gui/GUIController.h"

using namespace cheatscore::core;

void Cheat::Update(HANDLE hDriver, uintptr_t uClient)
{
	if (bState.load(std::memory_order_relaxed))
		CheatUpdate(hDriver, uClient);
}

const uintptr_t Cheat::getPlayerPawnByIndex(const HANDLE hDriver, const uintptr_t uClient, uintptr_t uEntityList, uint32_t uIndex) noexcept
{
	uintptr_t pEntityListEntry = driver::read<uintptr_t>(hDriver, uEntityList + (8 * (uIndex & 0x7FF) >> 9) + 16);
	if (!pEntityListEntry)
		return 0;

	uintptr_t uPlayerController = driver::read<uintptr_t>(hDriver, pEntityListEntry + 120 * (uIndex & 0x1FF));
	if (!uPlayerController)
		return 0;

	uintptr_t uPlayerPawnIndex = driver::read<uint32_t>(hDriver, uPlayerController + schemas::client_dll::CCSPlayerController::m_hPlayerPawn);
	if (!uPlayerPawnIndex)
		return 0;

	pEntityListEntry = driver::read<uintptr_t>(hDriver, uEntityList + (0x8 * ((uPlayerPawnIndex & 0x7FFF) >> 9) + 16));
	return driver::read<uintptr_t>(hDriver, pEntityListEntry + (120 * (uPlayerPawnIndex & 0x1FF)));
}

const uintptr_t Cheat::getLocalPlayerPawn(const HANDLE hDriver, const uintptr_t uClient) noexcept
{
	return driver::read<uintptr_t>(hDriver, uClient + offsets::client_dll::dwLocalPlayerPawn);
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

bool RadarHack::updateEntity(CSPlayerEntity& target, HANDLE hDriver, uintptr_t uClient, uintptr_t uPlayerPawn)
{
	uintptr_t uGameSceneNode = driver::read<uintptr_t>(hDriver, uPlayerPawn + schemas::client_dll::C_BaseEntity::m_pGameSceneNode);

	if (!uGameSceneNode)
		return false;

	uint8_t		uTeam = driver::read<uint8_t>(hDriver, uPlayerPawn + schemas::client_dll::C_BaseEntity::m_iTeamNum);
	if (uTeam == 0)
		return false;

	int32_t		uHealth = driver::read<int32_t>(hDriver, uPlayerPawn + schemas::client_dll::C_BaseEntity::m_iHealth);

	Vector3f	vSpot = driver::read<Vector3f>(hDriver, uGameSceneNode + schemas::client_dll::CGameSceneNode::m_vecAbsOrigin);
	QAngle		vRot = driver::read<QAngle>(hDriver, uGameSceneNode + schemas::client_dll::CGameSceneNode::m_angRotation);

	uint32_t	isSpotted = driver::read<uint32_t>(hDriver, uPlayerPawn + schemas::client_dll::C_CSPlayerPawn::m_entitySpottedState + schemas::client_dll::EntitySpottedState_t::m_bSpottedByMask);

	{
		target = CSPlayerEntity(0, "Player", uTeam, uHealth, 0, vSpot, {}, {}, vRot, isSpotted);
	}
	return true;
}


void RadarHack::fullSyncUpdate(HANDLE hDriver, uintptr_t uClient)
{
	vEntities.clear();

	uintptr_t uEntityList = driver::read<uintptr_t>(hDriver, uClient + offsets::client_dll::dwEntityList);

	if (!uEntityList)
		return;

	uintptr_t pLocalPlayer = getLocalPlayerPawn(hDriver, uClient);
	uintptr_t pLocalPlayerController = driver::read<uintptr_t>(hDriver, uClient + offsets::client_dll::dwLocalPlayerController);

	if (!pLocalPlayer)
		return;

	uintptr_t uLocalGameSceneNode = driver::read<uintptr_t>(hDriver, pLocalPlayer + schemas::client_dll::C_BaseEntity::m_pGameSceneNode);

	if (!uLocalGameSceneNode)
		return;

	if (updateEntity(localEntity.second, hDriver, uClient, pLocalPlayer))
	{
		localEntity.first = pLocalPlayer;
	}
	else return;

	for (uint32_t i = 0; i < 32; i++)
	{
		uintptr_t uPlayerPawn = getPlayerPawnByIndex(hDriver, uClient, uEntityList, i);
		
		if (!uPlayerPawn || uPlayerPawn == pLocalPlayer)
		{
			localEntity.second.uIndex = i;
			continue;
		}

		if (updateEntity(vEntities[uPlayerPawn], hDriver, uClient, uPlayerPawn))
		{
			vEntities[uPlayerPawn].uIndex = i;
		}
	}

	bNeedFullSync = false;
}

void RadarHack::CheatUpdate(HANDLE hDriver, uintptr_t uClient)
{
	if (bNeedFullSync)
	{
		fullSyncUpdate(hDriver, uClient);
		return;
	}

	uintptr_t uLocalGameSceneNode = driver::read<uintptr_t>(hDriver, localEntity.first + schemas::client_dll::C_BaseEntity::m_pGameSceneNode);

	if (!uLocalGameSceneNode)
		return;

	if (!updateEntity(localEntity.second, hDriver, uClient, localEntity.first))
	{
		return;
	}

	for (std::pair<uintptr_t, CSPlayerEntity> entityDesc : vEntities)
	{
		updateEntity(vEntities[entityDesc.first], hDriver, uClient, entityDesc.first);
	}
}

void RadarHack::Render()
{
	ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowBgAlpha(0.0f);
	ImGui::Begin("Radar", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);

	ImGui::Checkbox("Enable debug", &this->bShowDebugInfo);

	ImVec2 vSize = ImGui::GetWindowSize();
	ImVec2 vPosition = ImGui::GetWindowPos();

	ImVec2 position = { vPosition.x + vSize.x / 2, vPosition.y + vSize.y / 2 };

	ImDrawList* imDrawList = ImGui::GetWindowDrawList();

	localEntity.second.Render(imDrawList, position, -90);

	imDrawList->AddLine({ vPosition.x, vPosition.y + vSize.y / 2 }, { vPosition.x + vSize.x, vPosition.y + vSize.y / 2 }, ImColor(255, 255, 255, 100));
	imDrawList->AddLine({ vPosition.x + vSize.x / 2, vPosition.y }, { vPosition.x + vSize.x / 2, vPosition.y + vSize.y }, ImColor(255, 255, 255, 100));

	for (const std::pair<uintptr_t, CSPlayerEntity>& entityDesc : vEntities)
	{
		CSPlayerEntity entity = entityDesc.second;
		if (entity.uHealth == 0)
			continue;

		// 1. Count in-game distance
		Vector3f vDistance = entity.vPosition - localEntity.second.vPosition;
		vDistance.x *= -1;
		int multiplier = vDistance.x * vDistance.y;
		multiplier /= fabs(multiplier);
		// 2. Count angles
		float fAngleBetween = acos((entity.vPosition.x * localEntity.second.vPosition.x + entity.vPosition.y * localEntity.second.vPosition.y) / (magnitude(entity.vPosition) * magnitude(localEntity.second.vPosition)));
		float fGUIAngle = (((localEntity.second.qAngle.y - fAngleBetween)) + 90) * 3.14 / 180;

		Vector3f vRotated = rotate(vDistance, fGUIAngle);
		ImVec2 vGUIRotated = gameToGUIPoint(vRotated);
		entity.Render(imDrawList, vGUIRotated, localEntity.second.qAngle.y - entity.qAngle.y - 90);
	}

	ImGui::End();
	
	passedTime += ImGui::GetIO().DeltaTime;
	if (passedTime >= 1)
	{
		bNeedFullSync = true;
		passedTime = 0;
	}
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

	pEntityListEntry = driver::read<uintptr_t>(hDriver, uEntityList + (0x8 * ((iCrosshairEntityID & 0x7FFF) >> 9) + 16));

	uintptr_t uPlayerPawn = driver::read<uintptr_t>(hDriver, pEntityListEntry + (120 * (iCrosshairEntityID & 0x1FF)));

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

	uintptr_t pLocalPlayer = getLocalPlayerPawn(hDriver, uClient);

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

	ViewMatrix localViewMatrix = driver::read<ViewMatrix>(hDriver, uClient + offsets::client_dll::dwViewMatrix);

	float fClosestDistance = -1;

	for (uint32_t i = 0; i < 32; i++)
	{
		bool bIgnoreWalls = SettingsTab::getInstance()->ignoreWalls;

		uintptr_t uPlayerPawn = getPlayerPawnByIndex(hDriver, uClient, uEntityList, i);

		if (!uPlayerPawn)
			continue;
		if (uPlayerPawn == pLocalPlayer)
		{
			uLocalIndex = i;
			continue;
		}

		uintptr_t uGameSceneNode = driver::read<uintptr_t>(hDriver, uPlayerPawn + schemas::client_dll::C_BaseEntity::m_pGameSceneNode);

		if (!uGameSceneNode)
			continue;

		int32_t uHealth = driver::read<int32_t>(hDriver, uPlayerPawn + schemas::client_dll::C_BaseEntity::m_iHealth);
		if (uHealth <= 0 || uHealth > 100)
		{
			mEntities[uPlayerPawn] = { true, false, {} };
			continue;
		}
		uint8_t	uTeam = driver::read<uint8_t>(hDriver, uPlayerPawn + schemas::client_dll::C_BaseEntity::m_iTeamNum);
		if (uTeam == uLocalTeam)
			continue;

		uintptr_t pGameSceneNode = driver::read<uintptr_t>(hDriver, uPlayerPawn + schemas::client_dll::C_BaseEntity::m_pGameSceneNode);
		if (!pGameSceneNode)
			continue;

		uintptr_t pBonearray = driver::read<uintptr_t>(hDriver, pGameSceneNode + schemas::client_dll::CSkeletonInstance::m_modelState + 0x80);

		if (!pBonearray)
			return;

		Vector3f vHeadPosition = driver::read<Vector3f>(hDriver, pBonearray + (6 * 0x20));

		uint32_t isSpotted = 0;

		if (!bIgnoreWalls)
		{
			isSpotted = driver::read<uint32_t>(hDriver, uPlayerPawn + schemas::client_dll::C_CSPlayerPawn::m_entitySpottedState + schemas::client_dll::EntitySpottedState_t::m_bSpottedByMask);
		}
		Vector3f screenHead = worldToScreenPoint(localViewMatrix, vHeadPosition);
		screenHead.z = 0;
		float fDistance = distance(vWindowCenter, screenHead);

		mEntities[uPlayerPawn] = { true, true, screenHead };

		if (fClosestDistance < 0 || fDistance < fClosestDistance)
		{
			fClosestDistance = fDistance;
			vClosestHeadPosition = vHeadPosition;
			uClosestSpotted = isSpotted;
		}
	}

	if (fClosestDistance < 0)
		return;
	if (!(uClosestSpotted & (1 << uLocalIndex - 1) || SettingsTab::getInstance()->ignoreWalls))
		return;

	float fDistance = fClosestDistance;

	Vector3f vOffset = vClosestHeadPosition - vLocalHeadPosition;
	QAngle angle = toAngle(vClosestHeadPosition - vLocalHeadPosition);

	QAngle oldAngle = driver::read<QAngle>(hDriver, uClient + offsets::client_dll::dwViewAngles);
	QAngle delta = oldAngle - angle;

	if (magnitude(delta) > SettingsTab::getInstance()->aimbotMaxDistance / 2)
		return;

	QAngle newAngle = oldAngle - delta / SettingsTab::getInstance()->aimbotSmoothness;

	driver::write<QAngle>(hDriver, uClient + offsets::client_dll::dwViewAngles, newAngle);
}

void AimBot::Render() {
	ImDrawList* imBackground = ImGui::GetBackgroundDrawList();

	ImVec2 vSize = ImGui::GetIO().DisplaySize;
	imBackground->AddCircle({ vSize.x / 2, vSize.y / 2 }, SettingsTab::getInstance()->aimbotMaxDistance * 5, ImColor(255, 255, 255, 255), 0, 2);

	for (const std::pair<uintptr_t, CSData>& data : mEntities)
	{
		if (!data.second.bRender)
			continue;

		ImVec2 screenPosition = {
			data.second.vScreenPosition.x,
			data.second.vScreenPosition.y
		};

		if (
			(screenPosition.x < vWindowCenter.x * 2) &&
			(screenPosition.y < vWindowCenter.y * 2) &&
			(screenPosition.x > 0) &&
			(screenPosition.y > 0)
			)
			imBackground->AddLine(
				{ vWindowCenter.x, vWindowCenter.y },
				screenPosition,
				ImColor(255, 255, 255)
			);
	}
}

void Antirecoil::CheatUpdate(HANDLE hDriver, uintptr_t uClient)
{
	uintptr_t uLocalPlayer = driver::read<uintptr_t>(hDriver, uClient + offsets::client_dll::dwLocalPlayerPawn);
	iShotsFired = driver::read<int32_t>(hDriver, uLocalPlayer + schemas::client_dll::C_CSPlayerPawn::m_iShotsFired);

	uint32_t attack = driver::read<uint32_t>(hDriver, uClient + buttons::attack);

	if (iShotsFired > 1 && attack == MEM_PRESSED)
	{
		QAngle viewAngle = driver::read<QAngle>(hDriver, uClient + offsets::client_dll::dwViewAngles);
		QAngle aimPunchAngle = driver::read<QAngle>(hDriver, uLocalPlayer + schemas::client_dll::C_CSPlayerPawn::m_aimPunchAngle) * 2.f;

		QAngle newAngle = {
			viewAngle.x + oldPunch.x - aimPunchAngle.x,
			viewAngle.y + oldPunch.y - aimPunchAngle.y,
			0
		};

		if (newAngle.x > 89.f) newAngle.x = 89.f;
		if (newAngle.x < -89.f) newAngle.x = -89.f;
		if (newAngle.y > 180.f) newAngle.y -= 360.f;
		if (newAngle.y < -180.f) newAngle.y += 360.f;

		driver::write<QAngle>(hDriver, uClient + offsets::client_dll::dwViewAngles, newAngle);

		oldPunch = aimPunchAngle;
	}
	else
	{
		oldPunch = { 0, 0, 0 };
	}
}

void Antirecoil::Render()
{

}

void BoneESP::scanForBones()
{

}

void BoneESP::fullSyncRebuild(HANDLE hDriver, uintptr_t uClient)
{
	std::vector<CSEntity> entities = mEntities.load();
	entities.clear();

	uintptr_t uEntityList = driver::read<uintptr_t>(hDriver, uClient + offsets::client_dll::dwEntityList);

	if (!uEntityList)
		return;

	uintptr_t pLocalPlayer = getLocalPlayerPawn(hDriver, uClient);

	if (!pLocalPlayer)
		return;

	uintptr_t uLocalGameSceneNode = driver::read<uintptr_t>(hDriver, pLocalPlayer + schemas::client_dll::C_BaseEntity::m_pGameSceneNode);

	if (!uLocalGameSceneNode)
		return;

	uintptr_t uLocalPlayer = driver::read<uintptr_t>(hDriver, uClient + offsets::client_dll::dwLocalPlayerPawn);
	vLocalPosition = driver::read<Vector3f>(hDriver, pLocalPlayer + schemas::client_dll::C_BasePlayerPawn::m_vOldOrigin);
	viewAngle = driver::read<QAngle>(hDriver, uClient + offsets::client_dll::dwViewAngles);
	viewMatrix = driver::read<ViewMatrix>(hDriver, uClient + offsets::client_dll::dwViewMatrix);

	uLocalTeam = driver::read<uint8_t>(hDriver, pLocalPlayer + schemas::client_dll::C_BaseEntity::m_iTeamNum);

	for (uint32_t i = 0; i < 32; i++)
	{
		uintptr_t uPlayerPawn = getPlayerPawnByIndex(hDriver, uClient, uEntityList, i);

		if (!uPlayerPawn || uPlayerPawn == pLocalPlayer)
		{
			continue;
		}

		entities.push_back({uPlayerPawn});
	}
	mEntities.set(entities);
	bNeedFullSync = false;
}

void BoneESP::CheatUpdate(HANDLE hDriver, uintptr_t uClient)
{
	if (bNeedFullSync)
	{
		fullSyncRebuild(hDriver, uClient);
		return;
	}
	uintptr_t pLocalPlayer = driver::read<uintptr_t>(hDriver, uClient + offsets::client_dll::dwLocalPlayerPawn);
	vLocalPosition = driver::read<Vector3f>(hDriver, pLocalPlayer + schemas::client_dll::C_BasePlayerPawn::m_vOldOrigin);
	viewAngle = driver::read<QAngle>(hDriver, uClient + offsets::client_dll::dwViewAngles);
	viewMatrix = driver::read<ViewMatrix>(hDriver, uClient + offsets::client_dll::dwViewMatrix);
	uLocalTeam = driver::read<uint8_t>(hDriver, pLocalPlayer + schemas::client_dll::C_BaseEntity::m_iTeamNum);
	
	uintptr_t uEntityList = driver::read<uintptr_t>(hDriver, uClient + offsets::client_dll::dwEntityList);

	uint32_t uUpdated = 0;
	std::vector<CSEntity> entities = mEntities.load();
	for (CSEntity& entityDesc : entities)
	{
		uintptr_t uPlayerPawn = entityDesc.uPlayerPawn;
		uintptr_t pGameSceneNode = driver::read<uintptr_t>(hDriver, uPlayerPawn + schemas::client_dll::C_BaseEntity::m_pGameSceneNode);

		if (!pGameSceneNode)
		{
			bNeedFullSync = true;
			return;
		}

		int32_t	uHealth = driver::read<int32_t>(hDriver, uPlayerPawn + schemas::client_dll::C_BaseEntity::m_iHealth);

		Vector3f	vOldOrigin = driver::read<Vector3f>(hDriver, uPlayerPawn + schemas::client_dll::C_BasePlayerPawn::m_vOldOrigin);
		Vector3f    vHeadOffset = driver::read<Vector3f>(hDriver, uPlayerPawn + schemas::client_dll::C_BaseModelEntity::m_vecViewOffset);
		Vector3f	vHeadPosition = vOldOrigin + vHeadOffset;
		uint8_t		uTeam = driver::read<uint8_t>(hDriver, uPlayerPawn + schemas::client_dll::C_BaseEntity::m_iTeamNum);

		uintptr_t pBonearray = driver::read<uintptr_t>(hDriver, pGameSceneNode + schemas::client_dll::CSkeletonInstance::m_modelState + 0x80);

		BoneStructure bonearray;
		if (pBonearray)
		{
			for (std::list<BoneIndex> boneMatrix : BoneJointList::Skeleton)
				for (BoneIndex boneId : boneMatrix)
				{
					bonearray[boneId] = driver::read<Vector3f>(hDriver, pBonearray + (boneId * 0x20));
				}
		}

		entityDesc = {
			uPlayerPawn,
			vOldOrigin,
			vHeadPosition,
			uTeam,
			uHealth,
			bonearray,
			true
		};

		uUpdated++;
	}

	if (uUpdated < entities.size())
	{
		bNeedFullSync = true;
	}
	mEntities.set(entities);
}

void BoneESP::Render()
{
	ImDrawList* imBackground = ImGui::GetBackgroundDrawList();
	std::vector<CSEntity> entities = mEntities.load();
	for (CSEntity entity : entities)
	{
		if (entity.iHealth <= 0)
			continue;

		Vector3f vEntityPosition = entity.vOrigin;
		Vector3f vHeadPosition = entity.vHeadPosition;

		Vector3f vPoint = worldToScreenPoint(viewMatrix, vEntityPosition);
		Vector3f vHead = worldToScreenPoint(viewMatrix, vHeadPosition);
		
		float height = vPoint.y - vHead.y;
		float width = height / 2.8f;
		float* fEntColor = (entity.uTeam == 2) ? SettingsTab::getInstance()->tColor : SettingsTab::getInstance()->ctColor;
		ImColor imEntColor = ImColor(fEntColor[0], fEntColor[1], fEntColor[2], 1.f);

 		imBackground->AddRect(
			{
				vPoint.x - width,
				vPoint.y
			},
			{
				vHead.x + width,
				vHead.y
			},
			imEntColor, 0, 0, 2
		);

		imBackground->AddLine(
			{
				vHead.x - width,
				vHead.y - 25
			},
			{
				vHead.x + width,
				vHead.y - 25
			},
			ImColor(0, 0 ,0),
			7
		);

		float fComputedHealth = width * 2 * entity.iHealth / 100;

		imBackground->AddLine(
			{
				vHead.x - width,
				vHead.y - 25
			},
			{
				vHead.x - width + fComputedHealth,
				vHead.y - 25
			},
			imEntColor,
			7
		);

		/*
		* TODO
		* 
		* FROM THIS POINT STARTS COMPLETE FUCKING MESS
		* PLEASE REFATOR
		*/
		std::map<uint8_t, Vector3f> bones;

		for (std::pair<uint8_t, Vector3f> bone : entity.bones.mBones)
		{
			bones.insert(bone);
		}

		for (std::list<BoneIndex> boneMatrix : BoneJointList::Skeleton)
		{
			BoneIndex rootBone = BoneIndex::none;
			for (BoneIndex boneId : boneMatrix)
			{
				if (rootBone == BoneIndex::none)
				{
					rootBone = boneId;
				}
				else
				{
					Vector3f boneStart = worldToScreenPoint(viewMatrix, bones[boneId]);
					Vector3f boneEnd = worldToScreenPoint(viewMatrix, bones[rootBone]);

					ImVec2 start = { boneStart.x, boneStart.y };
					ImVec2 end = { boneEnd.x, boneEnd.y };

					imBackground->AddLine(
						start, 
						end, 
						ImColor(255, 255, 255, 255), 
						2
					);

					rootBone = boneId;
				}
			}
		}
	}

	passedTime += ImGui::GetIO().DeltaTime;
	if (passedTime >= 1)
	{
		bNeedFullSync = true;
		passedTime = 0;
	}
}

void Autostrafe::CheatUpdate(HANDLE hDriver, uintptr_t uClient)
{

}

void Autostrafe::Render()
{

}