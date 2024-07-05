#pragma once
#include <vector>
#include <cmath>
#include "../../imgui/imgui.h"

#include "ThreadController.h"
#include "GUIEntities.h"
#include "../km_mailbox.h"
#include "../utility/custom_types.h"
#include "../utility/CheatUtilities.h"

struct CSEntity
{
	Vector3f	pos;
	uint8_t		uTeam = 0;
	float		rotation;
};

class Cheat : public ThreadedObject
{
private:
	inline static std::vector<Cheat*> instances;
	uint32_t uPlace;
public:
	static std::vector<Cheat*> Instances() { return instances; }

	Cheat() { 
		this->uPlace = instances.size();
		instances.push_back(this);
	}

	~Cheat() { instances.erase(instances.begin() + uPlace); }
	virtual void Render(ImDrawList*) = 0;
};

class BhopCheat : public Cheat
{
public:
	void Update(HANDLE, uintptr_t) override;

	void Render(ImDrawList*) override;
};

class RadarHack : public Cheat
{
private:
	std::vector<RadarEntity> vEntities;
	
	RadarEntity localEntity;
	
	bool bShowDebugInfo = false;
	bool bInitialised = false;
public:
	bool Initialized() { return bInitialised; }
	void Update(HANDLE, uintptr_t) override;

	void Render(ImDrawList*) override;
};
