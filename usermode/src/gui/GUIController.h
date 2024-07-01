#pragma once
#include <imgui.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>
#include <d3d9.h>
#include <vector>

#include <Windows.h>

#include "../core/km_mailbox.h"
#include "../core/cheat/Cheat.h"

#pragma comment(lib,"d3d9.lib")


class GUIController
{
private:
	inline static GUIController* instance;

	ImGuiIO io;
	ImVec4 clear_color = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);

	bool bAttached = false;
	bool bEnableChildren = false;
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
