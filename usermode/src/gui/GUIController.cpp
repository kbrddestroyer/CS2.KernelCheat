#include "GUIController.h"

using namespace cheatscore::core;
using namespace cheatscore::utility;

extern bool menuShow;
extern bool safeExit;

void ApplyTheme(int themeIndex)
{
    switch (themeIndex)
    {
    case 0:
        ImGui::StyleColorsDark();
        break;
    case 1:
        ImGui::StyleColorsLight();
        break;
    case 2:
        ImGui::StyleColorsClassic();
        break;
    case 3:
        ImGui::StyleColorsModernApple();
        break;
    default:
        ImGui::StyleColorsDark();
        break;
    }
}

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
    if (menuShow)
    {
        ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
        if (ImGui::Begin("kbrddestroyer kernel | cs2 | external", nullptr, ImGuiWindowFlags_NoCollapse))
        {
            if (ImGui::BeginTabBar("##tabs"))
            {
                if (ImGui::BeginTabItem("Settings"))
                {
                    settings.Render();
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Theme"))
                {
                    static int themeIndex = 0;
                    const char* themes[] = { "Dark", "Light", "Classic", "Modern Apple Light" };

                    ImGui::Text("Select Theme:");
                    if (ImGui::Combo("Theme", &themeIndex, themes, IM_ARRAYSIZE(themes)))
                    {
                        ApplyTheme(themeIndex);
                    }

                    ImGui::Text("Window Size");
                    ImGui::SliderFloat2("Size", (float*)&windowSize, 400.0f, 1200.0f);

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

    if (ImGui::Checkbox("Radar Enabled", &this->radarhackEnabled))
    {
        if (!Cheat::Instances(CheatEntities::RADAR))
            ThreadedObject::createObject(std::make_shared<RadarHack>());
        else Cheat::Instances(RADAR)->toggle(this->radarhackEnabled);
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
        else Cheat::Instances(BHOP)->toggle(this->bhopEnabled);
    }

    ImGui::Separator();
    ImGui::Text("Trigger Bot");
    ImGui::Separator();

    if (ImGui::Checkbox("Trigger Enabled", &this->triggerEnabled))
    {
        if (!Cheat::Instances(CheatEntities::TRIGGER))
            ThreadedObject::createObject(std::make_shared<TriggerBot>());
        else Cheat::Instances(TRIGGER)->toggle(this->triggerEnabled);
    }

    ImGui::SliderInt("Trigger Delay", &this->triggerDelay, 10, 250);

    ThreadMgr::getInstance()->getMutex().unlock();
}

void SettingsTab::Update()
{
}