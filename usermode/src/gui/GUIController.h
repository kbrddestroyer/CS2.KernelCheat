#pragma once
#include <imgui.h>
#include <vector>

#include <Windows.h>

#include "../core/km_mailbox.h"
#include "../core/cheat/Cheat.h"

class ChildGUIController;

class GUIController
{
private:
	inline static GUIController* instance;

	ImGuiIO io;
	ImVec4 clear_color = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);

	bool bAttached = false;
	bool bEnableChildren = false;

	std::vector<std::shared_ptr<ChildGUIController>> vChildren;
protected:
	virtual void Initialize();
public:
	GUIController();
	GUIController(ImGuiIO&);
	~GUIController() { instance = nullptr; }

	virtual void Render();
	virtual void Update();
	ImVec4 getClearColor() { return clear_color; }

	static GUIController* Instance() { return instance; }
};

class ChildGUIController
{
protected:
	bool		bEnabled = false;
	ImVec2		vWndPos;
	ImDrawList*	imDrawList;
public:
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