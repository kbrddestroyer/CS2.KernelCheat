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
    controller.name = "Radar";
    settings.name = "Settings";
}

void GUIController::Update()
{
    for (std::shared_ptr<ChildGUIController> controller : vChildren)
        controller->Render();
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

        ImGui::End();

        controller.InternalUpdate();
        settings.InternalUpdate();
    }
#pragma endregion
    Update();
    ImGui::End();
}

void ChildGUIController::InternalUpdate()
{
    ImGui::Begin(name.c_str());
    imDrawList = ImGui::GetWindowDrawList();
    vWndPos = ImGui::GetWindowPos();
    Render();
    Update();

    ImGui::End();
}

void CheatRenderer::Render()
{
    for (Cheat* instance : Cheat::Instances())
    {
        if (instance)
        {
            instance->Render(imDrawList);
        }
    }
}

void CheatRenderer::Update()
{
}

void SettingsTab::Render()
{
    ThreadMgr::getInstance()->getMutex().lock();
    ImGui::Text("Radarhack section");
    ImGui::Separator();

    ImGui::Checkbox("Enabled", &this->radarhackEnabled);
    ImGui::ColorEdit4("CT Color", ctColor);
    ImGui::ColorEdit4("T Color", tColor);

    ThreadMgr::getInstance()->getMutex().unlock();
}

void SettingsTab::Update()
{

}