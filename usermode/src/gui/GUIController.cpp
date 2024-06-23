#include "GUIController.h"

GUIController::GUIController()
{
    throw "Call GUIController constructor without parameter";
}

GUIController::GUIController(ImGuiIO& io)
{
    this->io = io;
	Initialize();
}

void GUIController::Initialize()
{

}

void GUIController::Update()
{

}

void GUIController::Render()
{
    ImGui::Begin("GUI Settigns");                          // Create a window called "Hello, world!" and append into it.
    ImGui::ColorEdit3("Background color", (float*)&clear_color); // Edit 3 floats representing a color

    ImGui::Text("INFORMATION:");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    
    Update();
    ImGui::End();
}