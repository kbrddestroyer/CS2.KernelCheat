#pragma once
#include <imgui.h>
#include <vector>
#include <cmath>

#include <atomic>

#include <Windows.h>

#include "../core/km_mailbox.h"
#include "../core/cheat/Cheat.h"

class ChildGUIController
{
protected:
	std::atomic<bool> bEnabled;
	ImVec2		vWndPos = {};
	ImDrawList* imDrawList = nullptr;
public:
	std::string name = "";
public:
	ChildGUIController() { bEnabled.store(false); }

	void InternalUpdate();

	virtual void Render() = 0;
	virtual void Update() = 0;
};

class CheatRenderer : public ChildGUIController
{
public:
	void Render() override;
	void Update() override;
};

class SettingsTab : public ChildGUIController
{
private:
	inline static SettingsTab* instance;
public:

	// Radarhack
	bool	radarhackEnabled = false;

	float ctColor[4] = { 0, 0, 255, 255 };
	float tColor[4] = { 255, 0, 0, 255 };

	// Bhop
	bool	bhopEnabled = false;

	// Trigger
	bool	triggerEnabled = false;
	int		triggerDelay = 10;

	// Aimbot
	bool	aimbotEnabled = false;
	bool	ignoreWalls = false;
	float	aimbotMaxDistance = 0.0f;
	float	aimbotSmoothness = 1.0f;

	bool	antirecoilEnabled = false;
	bool	wallhackEnabled = false;
	bool	counterstrafeEnabled = false;
public:
	static SettingsTab* getInstance() { return instance; }

	SettingsTab() { instance = this; }
	~SettingsTab() { instance = nullptr; }

	void Render() override;
	void Update() override;
};

class GUIController
{
private:
	inline static GUIController* instance;

	ImGuiIO io;
	ImVec4 clear_color = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);

	bool bAttached = true;
	bool bEnableChildren = false;
	bool bSafeExit = false;

	SettingsTab settings;	
	CheatRenderer controller;
protected:
	std::atomic<bool> menuShow = true;
	virtual void Initialize();
public:
	GUIController();
	GUIController(ImGuiIO&);
	~GUIController() { instance = nullptr; }

	virtual void Render();
	virtual void Update();

	ImVec4 getClearColor() { return clear_color; }

	void toggle(bool bState) { menuShow.store(bState); }
	bool getState() { return menuShow.load(std::memory_order_relaxed); }

	void safeExit(bool bState) { this->bSafeExit = bState; }
	bool safeExit() { return bSafeExit; }

	static GUIController* Instance() { return instance; }
};
