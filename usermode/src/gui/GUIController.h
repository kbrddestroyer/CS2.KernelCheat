#pragma once
#include <imgui.h>

class GUIController
{
private:
	ImGuiIO io;

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	float f = 0.f;
	unsigned int counter = 0;
protected:
	void Initialize();
public:
	GUIController();
	GUIController(ImGuiIO&);

	void Render();
	ImVec4 getClearColor() { return clear_color; }
};
