#include "GUIController.h"

using namespace cheatscore::core;
using namespace cheatscore::utility;

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
    for (std::pair<CheatEntities, Cheat*> instance : Cheat::getMap())
    {
        if (instance.second)
        {
            instance.second->Render(imDrawList);
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

    if (ImGui::Checkbox("Radar Enabled", &this->radarhackEnabled))
    {
        if (!Cheat::Instances(CheatEntities::RADAR))
            ThreadedObject::createObject(std::make_shared<RadarHack>());
    }
    ImGui::ColorEdit4("CT Color", ctColor);
    ImGui::ColorEdit4("T Color", tColor);

    ImGui::Separator();
    ImGui::Text("Bunnyhop section");
    ImGui::Separator();

    if (ImGui::Checkbox("Bhop Enabled", &this->bhopEnabled))
    {
        if (!Cheat::Instances(CheatEntities::BHOP))
            ThreadedObject::createObject(std::make_shared<BhopCheat>());
    }

    ImGui::Separator();
    ImGui::Text("Trigger section");
    ImGui::Separator();

    if (ImGui::Checkbox("Trigger Enabled", &this->triggerEnabled))
    {
        if (!Cheat::Instances(CheatEntities::TRIGGER))
            ThreadedObject::createObject(std::make_shared<TriggerBot>());
    }

    ImGui::SliderInt("Trigger discipline", &this->triggerDelay, 10, 250);

    ThreadMgr::getInstance()->getMutex().unlock();
}

void SettingsTab::Update()
{

}