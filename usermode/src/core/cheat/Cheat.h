#pragma once
#include <vector>
#include "../../imgui/imgui.h"

#include "ThreadController.h"
#include "../km_mailbox.h"

class Cheat : public ThreadedObject
{
	virtual void Render() = 0;
};

class BhopCheat : public Cheat
{
public:
	void Update(HANDLE, uintptr_t) override;

	void Render() override;
};

class RadarHack : public Cheat
{
private:
	std::vector<uintptr_t> vEntities;

	bool bInitialised = false;
public:
	bool Initialized() { return bInitialised; }
	void Update(HANDLE, uintptr_t) override;

	void Render() override;

	std::vector<uintptr_t> Entities() { return vEntities; }
};
