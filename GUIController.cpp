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
    io = ImGui::GetIO();
    io.IniFilename = nullptr;
    Initialize();
}

void GUIController::Initialize()
{
    controller.name = "Radar";
    settings.name = "Settings";
}

void GUIController::Update()
{

}

void GUIController::Render()
{
    io.MouseDrawCursor = menuShow;
    settings.Update();
    if (menuShow)
    {
        ImGui::SetNextWindowSize(ImVec2(667, 400), ImGuiCond_Always);
        if (ImGui::Begin("kbrddestroyer kernel | cs2 | external", nullptr, ImGuiWindowFlags_NoCollapse))
        {
            settings.Render();
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
    for (const std::pair<CheatEntities, Cheat*>& instance : Cheat::getMap())
    {
        if (instance.second && instance.second->enabled())
        {
            instance.second->Render();
        }
    }
}

void CheatRenderer::Update()
{
    // No update but needs to be overriden
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
    if (ImGui::Button("Safe exit"))
    {
        exit(0);
    }

    if (ImGui::BeginTabBar("CheatTabs"))
    {
        if (ImGui::BeginTabItem("Aimbot"))
        {
            ImGui::Text("Aimbot");
            ImGui::Separator();

            if (ImGui::Checkbox("Aim Enabled", &this->aimbotEnabled))
            {
                if (!Cheat::Instances(CheatEntities::AIMBOT))
                    ThreadedObject::createObject(std::make_shared<AimBot>());
                Cheat::Instances(AIMBOT)->toggle(this->aimbotEnabled);
            }
            ImGui::Checkbox("Aim Walls", &this->ignoreWalls);
            ImGui::SliderFloat("Aimbot Max Angle", &this->aimbotMaxDistance, 1, 280);
            ImGui::SliderFloat("Aimbot Smooth", &this->aimbotSmoothness, 1, 250);

            ImGui::Separator();
            ImGui::Text("TriggerBot");
            ImGui::Separator();

            if (ImGui::Checkbox("Trigger Enabled", &this->triggerEnabled))
            {
                if (!Cheat::Instances(CheatEntities::TRIGGER))
                    ThreadedObject::createObject(std::make_shared<TriggerBot>());
                Cheat::Instances(TRIGGER)->toggle(this->triggerEnabled);
            }

            ImGui::SliderInt("Trigger Delay", &this->triggerDelay, 10, 250);

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Visuals"))
        {
            ImGui::Text("Wallhack");
            ImGui::Separator();

            if (ImGui::Checkbox("Wallhack", &this->wallhackEnabled))
            {
                if (!Cheat::Instances(CheatEntities::BONE_ESP))
                    ThreadedObject::createObject(std::make_shared<BoneESP>());
                Cheat::Instances(BONE_ESP)->toggle(this->wallhackEnabled);
            }

            ImGui::Separator();
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

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("RCS"))
        {
            ImGui::Text("Antirecoil");
            ImGui::Separator();

            if (ImGui::Checkbox("Antirecoil", &this->antirecoilEnabled))
            {
                if (!Cheat::Instances(CheatEntities::ANTIRECOIL))
                    ThreadedObject::createObject(std::make_shared<Antirecoil>());
                Cheat::Instances(ANTIRECOIL)->toggle(this->antirecoilEnabled);
            }

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Misc"))
        {
            ImGui::Text("BunnyHop");
            ImGui::Separator();

            if (ImGui::Checkbox("BunnyHop Enabled", &this->bhopEnabled))
            {
                if (!Cheat::Instances(CheatEntities::BHOP))
                    ThreadedObject::createObject(std::make_shared<BhopCheat>());
                Cheat::Instances(BHOP)->toggle(this->bhopEnabled);
            }

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }
}