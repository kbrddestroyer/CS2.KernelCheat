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

class RadarEntity : public GUIEntity
{
public:
	std::string		sName;
	std::uint8_t	uTeam;
	std::uint32_t	uHealth;
	std::uint32_t	uArmor;
	bool			hasHelmet;
	bool			isBombCarrier;
	bool			isLocal;

	Vector3f		vPosition;
	QAngle			qAngle;

	Vector3f		vOffset = { 1, -20 };
public:
	RadarEntity() {}
	RadarEntity(std::string, std::uint8_t, std::uint32_t, std::uint32_t, Vector3f, QAngle, bool=false, bool=false, bool=false);
	void setName(std::string name) { this->sName = name; }
	void Render(ImDrawList*, ImVec2, float) override;

	ImVec2 getEntityPosition(Vector3f, Vector3f, float);
};
