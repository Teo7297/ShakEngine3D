#include "UIManager.h"

using namespace Shak;

void UIManager::Update(float deltaTime) // TODO: this should call update on the active UI layer
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    bool showDemo = true;
    ImGui::ShowDemoWindow(&showDemo);
    ImGui::Render();
}

void UIManager::ProcessEvent(SDL_Event event)
{

}
