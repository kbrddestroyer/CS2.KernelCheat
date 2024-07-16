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
    if (settings.radarhackEnabled)
    {
        ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowBgAlpha(0.0f);
        ImGui::Begin("Radar", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);
        controller.InternalUpdate();
        ImGui::End();
    }
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

    ImGui::Text("Radar Hack");
    ImGui::Separator();

    if (GetAsyncKeyState(VK_F1) & 1)
    {
        this->radarhackEnabled = !this->radarhackEnabled;

        if (!Cheat::Instances(CheatEntities::RADAR))
            ThreadedObject::createObject(std::make_shared<RadarHack>());
        Cheat::Instances(RADAR)->toggle(this->radarhackEnabled);
    }

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

    if (GetAsyncKeyState(VK_F2) & 1)
    {
        this->bhopEnabled = !this->bhopEnabled;

        if (!Cheat::Instances(CheatEntities::BHOP))
            ThreadedObject::createObject(std::make_shared<BhopCheat>());
        Cheat::Instances(BHOP)->toggle(this->bhopEnabled);
    }

    if (ImGui::Checkbox("Bhop Enabled", &this->bhopEnabled))
    {
        if (!Cheat::Instances(CheatEntities::BHOP))
            ThreadedObject::createObject(std::make_shared<BhopCheat>());
        Cheat::Instances(BHOP)->toggle(this->bhopEnabled);
    }

    ImGui::Separator();
    ImGui::Text("Trigger Bot");
    ImGui::Separator();

    if (GetAsyncKeyState(VK_F3) & 1)
    {
        this->triggerEnabled = !this->triggerEnabled;

        if (!Cheat::Instances(CheatEntities::TRIGGER))
            ThreadedObject::createObject(std::make_shared<TriggerBot>());
        Cheat::Instances(TRIGGER)->toggle(this->triggerEnabled);
    }

    if (ImGui::Checkbox("Trigger Enabled", &this->triggerEnabled))
    {
        if (!Cheat::Instances(CheatEntities::TRIGGER))
            ThreadedObject::createObject(std::make_shared<TriggerBot>());
        Cheat::Instances(TRIGGER)->toggle(this->triggerEnabled);
    }

    ImGui::SliderInt("Trigger Delay", &this->triggerDelay, 10, 250);

    if (GetAsyncKeyState(VK_OEM_PLUS) & 1)
    {
        this->triggerDelay = std::clamp(this->triggerDelay + 10, 10, 250);
    }

    if (GetAsyncKeyState(VK_OEM_MINUS) & 1)
    {
        this->triggerDelay = std::clamp(this->triggerDelay - 10, 10, 250);
    }

    ThreadMgr::getInstance()->getMutex().unlock();
}

void SettingsTab::Update()
{
}