#pragma once
#include <vector>
#include "../../imgui/imgui.h"

#include "ThreadController.h"
#include "../km_mailbox.h"

class BhopCheat : public ThreadedObject
{
public:
	void Update(HANDLE, uintptr_t) override;

	void Render();
};

class RadarHack : public ThreadedObject
{
private:
	std::vector<uintptr_t> vEntities;

	bool bInitialised = false;
public:
	bool Initialized() { return bInitialised; }
	void Update(HANDLE, uintptr_t) override;

	void Render();

	std::vector<uintptr_t> Entities() { return vEntities; }
};
