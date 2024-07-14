#pragma once
#include <string>

#include "../../imgui/imgui.h"
#include "../utility/custom_types.h"
#include "../utility/CheatUtilities.h"

class GUIEntity
{
public:
	virtual void Render(ImDrawList*, ImVec2, float) = 0;
};

class CSPlayerEntity : public GUIEntity
{
public:
	std::string		sName;
	std::uint8_t	uTeam;
	std::uint32_t	uHealth;
	std::uint32_t	uArmor;
	std::uint32_t	uIndex;
	bool			hasHelmet;
	std::uint32_t	isSpotted;
	bool			isBombCarrier;
	bool			isLocal;

	Vector3f		vPosition;
	Vector3f		vOldPosition;
	Vector3f		vHeadPosition;
	QAngle			qAngle;

	Vector3f		vOffset = { 1, -20 };
public:
	CSPlayerEntity() {}
	CSPlayerEntity(uint32_t, std::string, std::uint8_t, std::uint32_t, std::uint32_t, Vector3f, Vector3f, Vector3f, QAngle, uint32_t=0, bool=false, bool=false, bool=false);
	void setName(std::string name) { this->sName = name; }
	void Render(ImDrawList*, ImVec2, float) override;

	ImVec2 getEntityPosition(Vector3f, Vector3f, float);
};
