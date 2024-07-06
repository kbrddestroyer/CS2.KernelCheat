#pragma once
#include <vector>
#include <map>
#include <cmath>
#include "../../imgui/imgui.h"

#include "ThreadController.h"
#include "GUIEntities.h"
#include "../km_mailbox.h"
#include "../utility/custom_types.h"
#include "../utility/CheatUtilities.h"

enum CheatEntities
{
	NONE,
	BHOP,
	RADAR
};

class Cheat : public ThreadedObject
{
private:
	inline static std::map<CheatEntities, Cheat*> instances;
	CheatEntities desc;
public:
	static std::map<CheatEntities, Cheat*>& getMap() { return instances; }
	static Cheat* Instances(CheatEntities entity) {
		return instances[entity]; 
	}

	Cheat(CheatEntities entity = CheatEntities::NONE) {
		if (entity == NONE)
			throw "Entity type cannot be none";

		instances[entity] = this;
		desc = entity;
	}

	~Cheat() { instances.erase(desc); }
	virtual void Render(ImDrawList*) = 0;
};

class BhopCheat : public Cheat
{
public:
	BhopCheat() : Cheat(BHOP) {}

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
	RadarHack() : Cheat(RADAR) {}

	bool Initialized() { return bInitialised; }
	void Update(HANDLE, uintptr_t) override;

	void Render(ImDrawList*) override;
};
