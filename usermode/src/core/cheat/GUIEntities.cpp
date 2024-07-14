#include "GUIEntities.h"
#include "../gui/GUIController.h"

CSPlayerEntity::CSPlayerEntity(uint32_t uIndex, std::string sName, std::uint8_t uTeam, std::uint32_t uHealth, std::uint32_t uArmor, Vector3f vPos, Vector3f vOldPos, Vector3f vHeadPosition, QAngle qRot, uint32_t isSpotted, bool isLocal, bool hasHelmet, bool isBombCarrier)
{
	this->uIndex		= uIndex;
	this->sName			= sName;
	this->uTeam			= uTeam;
	this->uHealth		= uHealth;
	this->uArmor		= uArmor;
	this->hasHelmet		= hasHelmet;
	this->isBombCarrier = isBombCarrier;
	this->isLocal		= isLocal;
	this->vPosition		= vPos;
	this->vOldPosition	= vOldPos;
	this->vHeadPosition	= vHeadPosition;
	this->qAngle		= qRot;
	this->isSpotted		= isSpotted;
}

void CSPlayerEntity::Render(ImDrawList* imDrawList, ImVec2 entityPosition, float entityRotation)
{
	ImVec2 mapped = entityPosition;
	ImVec2 mappedDir = { mapped.x + cos(entityRotation * (3.14f / 180)) * 50, mapped.y + sin(entityRotation * (3.14f / 180)) * 50 };

	float* fEntColor = (uTeam == 2) ? SettingsTab::getInstance()->tColor : SettingsTab::getInstance()->ctColor;

	ImColor imEntColor = ImColor(fEntColor[0], fEntColor[1], fEntColor[2], fEntColor[3]);

	imDrawList->AddRect(
		gameToGUIPoint({ -MAX_MAX_COORD, -MAX_MAX_COORD, 0 }),
		gameToGUIPoint({ MAX_MAX_COORD, MAX_MAX_COORD, 0 }),
		ImColor(255, 255, 255),
		1
	);
	imDrawList->AddCircleFilled(mapped, 5, imEntColor);
	imDrawList->AddLine(mapped, mappedDir, imEntColor, 3);

	ImVec2 textPos = { mapped.x + vOffset.x, mapped.y + vOffset.y };

	imDrawList->AddText(textPos, ImColor(255, 255, 255), sName.c_str());
}

ImVec2 CSPlayerEntity::getEntityPosition(Vector3f position, Vector3f localPlayerPosition, float rotation)
{
	return GUIPointToLocalObserver(position, localPlayerPosition, rotation);
}
