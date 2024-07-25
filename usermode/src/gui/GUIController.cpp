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
    io.MouseDrawCursor = menuShow;
    settings.Update();
    if (menuShow)
    {
        if (ImGui::Begin("kbrddestroyer kernel | cs2 | external", nullptr, ImGuiWindowFlags_NoCollapse))
        {
            if (ImGui::BeginTabBar("##tabs"))
            {
                if (ImGui::BeginTabItem("Settings"))
                {
                    settings.Render();
                    ImGui::EndTabItem();
                }

                ImGui::EndTabBar();
            }

            ImGui::End();
        }
    }
    controller.InternalUpdate();
}

void ChildGUIController::InternalUpdate()
{
    Render();
    Update();
}

void CheatRenderer::Render()
{
    for (std::pair<CheatEntities, Cheat*> instance : Cheat::getMap())
    {
        if (instance.second && instance.second->enabled())
        {
            instance.second->Render();
        }
    }
}

void CheatRenderer::Update()
{

}


void SettingsTab::Update()
{
    if (GetAsyncKeyState(VK_F1) & 1)
    {
        this->radarhackEnabled = !this->radarhackEnabled;

        if (!Cheat::Instances(CheatEntities::RADAR))
            ThreadedObject::createObject(std::make_shared<RadarHack>());
        Cheat::Instances(RADAR)->toggle(this->radarhackEnabled);
    }
    if (GetAsyncKeyState(VK_F2) & 1)
    {
        this->bhopEnabled = !this->bhopEnabled;

        if (!Cheat::Instances(CheatEntities::BHOP))
            ThreadedObject::createObject(std::make_shared<BhopCheat>());
        Cheat::Instances(BHOP)->toggle(this->bhopEnabled);
    }
    if (GetAsyncKeyState(VK_F3) & 1)
    {
        this->triggerEnabled = !this->triggerEnabled;

        if (!Cheat::Instances(CheatEntities::TRIGGER))
            ThreadedObject::createObject(std::make_shared<TriggerBot>());
        Cheat::Instances(TRIGGER)->toggle(this->triggerEnabled);
    }

    if (GetAsyncKeyState(VK_OEM_PLUS) & 1)
    {
        this->triggerDelay = std::clamp(this->triggerDelay + 10, 10, 250);
    }

    if (GetAsyncKeyState(VK_OEM_MINUS) & 1)
    {
        this->triggerDelay = std::clamp(this->triggerDelay - 10, 10, 250);
    }

    if (GetAsyncKeyState(VK_F4) & 1)
    {
        if (!Cheat::Instances(CheatEntities::AIMBOT))
            ThreadedObject::createObject(std::make_shared<AimBot>());
        Cheat::Instances(AIMBOT)->toggle(this->aimbotEnabled);
    }
}

void SettingsTab::Render()
{
    ImGui::Text("Radar Hack");
    ImGui::Separator();

    if (ImGui::Checkbox("Radar Enabled", &this->radarhackEnabled))
    {
        if (!Cheat::Instances(CheatEntities::RADAR))
            ThreadedObject::createObject(std::make_shared<RadarHack>());
        Cheat::Instances(RADAR)->toggle(this->radarhackEnabled);
    }

    ImGui::ColorEdit4("CT Color", ctColor);
    ImGui::ColorEdit4("T Color", tColor);

    ImGui::Separator();
    ImGui::Text("BunnyHop");
    ImGui::Separator();

    if (ImGui::Checkbox("Bhop Enabled", &this->bhopEnabled))
    {
        if (!Cheat::Instances(CheatEntities::BHOP))
            ThreadedObject::createObject(std::make_shared<BhopCheat>());
        Cheat::Instances(BHOP)->toggle(this->bhopEnabled);
    }

    ImGui::Separator();
    ImGui::Text("Trigger Bot");
    ImGui::Separator();

    if (ImGui::Checkbox("Trigger Enabled", &this->triggerEnabled))
    {
        if (!Cheat::Instances(CheatEntities::TRIGGER))
            ThreadedObject::createObject(std::make_shared<TriggerBot>());
        Cheat::Instances(TRIGGER)->toggle(this->triggerEnabled);
    }

    ImGui::SliderInt("Trigger Delay", &this->triggerDelay, 10, 250);

    ImGui::Separator();
    ImGui::Text("Aimbot section");

    if (ImGui::Checkbox("Aim Enabled", &this->aimbotEnabled))
    {
        if (!Cheat::Instances(CheatEntities::AIMBOT))
            ThreadedObject::createObject(std::make_shared<AimBot>());
        Cheat::Instances(AIMBOT)->toggle(this->aimbotEnabled);
    }
    ImGui::Checkbox("Aim Walls", &this->ignoreWalls);
    ImGui::SliderFloat("Aimbot Max Angle", &this->aimbotMaxDistance, 1, 360);
    ImGui::SliderFloat("Aimbot Smooth", &this->aimbotSmoothness, 1, 125);

    ImGui::Separator();
    ImGui::Text("Antirecoil");

    if (ImGui::Checkbox("Antirecoil", &this->antirecoilEnabled))
    {
        if (!Cheat::Instances(CheatEntities::ANTIRECOIL))
            ThreadedObject::createObject(std::make_shared<Antirecoil>());
        Cheat::Instances(ANTIRECOIL)->toggle(this->antirecoilEnabled);
    }
}
