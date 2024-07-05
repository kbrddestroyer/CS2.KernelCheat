#include "GUIEntities.h"
#include "../gui/GUIController.h"

RadarEntity::RadarEntity(std::string sName, std::uint8_t uTeam, std::uint32_t uHealth, std::uint32_t uArmor, Vector3f vPos, QAngle qRot, bool isLocal, bool hasHelmet, bool isBombCarrier)
{
	this->sName			= sName;
	this->uTeam			= uTeam;
	this->uHealth		= uHealth;
	this->uArmor		= uArmor;
	this->hasHelmet		= hasHelmet;
	this->isBombCarrier = isBombCarrier;
	this->isLocal		= isLocal;
	this->vPosition		= vPos;
	this->qAngle		= qRot;
}

void RadarEntity::Render(ImDrawList* imDrawList, ImVec2 entityPosition, float entityRotation)
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

ImVec2 RadarEntity::getEntityPosition(Vector3f position, Vector3f localPlayerPosition, float rotation)
{
	return GUIPointToLocalObserver(position, localPlayerPosition, rotation);
}
