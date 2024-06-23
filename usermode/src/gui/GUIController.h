#pragma once
#include <imgui.h>

#include "../core/km_mailbox.h"

class GUIController
{
private:
	ImGuiIO io;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	bool bAttached = false;
protected:
	virtual void Initialize();
public:
	GUIController();
	GUIController(ImGuiIO&);

	void Render();
	virtual void Update();
	
	ImVec4 getClearColor() { return clear_color; }
};
