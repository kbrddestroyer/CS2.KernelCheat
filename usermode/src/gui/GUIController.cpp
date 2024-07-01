#include "GUIController.h"

GUIController::GUIController()
{
    throw "Call GUIController constructor without parameter";
}

GUIController::GUIController(ImGuiIO& io)
{
    this->io = io;
    instance = this;
    //vChildGUIs.push_back(std::make_shared<OverlayController>(io));
    
	Initialize();
}

void GUIController::Initialize()
{
    // Nothing to handle here yet
}

void GUIController::Update()
{
    // Nothing to handle here yet

    for (std::shared_ptr pChild : vChildGUIs)
    {
        pChild->Render();
    }
}

void GUIController::Render()
{
#pragma region SETTINGS_TAB
    {   
        ImGui::Begin("GUI Settigns");
        ImGui::ColorEdit3("Background color", (float*)&clear_color);

        ImGui::Text("INFORMATION:");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    }
#pragma endregion
    
#pragma region CORE_CONTROL_TAB
    {
        ImGui::Begin("Core");
        if (ImGui::Button("ATTACH"))
        {
            if (kmControllerEntry() == EXIT_SUCCESS)
            {
                bAttached = true;
            }
        }
        if (ImGui::Checkbox("Enable overlay", &this->bEnableChildren))
        {
        }

        ImGui::End();
    }
#pragma endregion
    Update();
    ImGui::End();
}
