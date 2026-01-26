#include "UI/UIManager.h"
#include "Renderer.h"

using namespace Shak;

void UIManager::Update(float deltaTime)
{
    if(m_activeLayer)
        m_activeLayer->Update(deltaTime);
}

void UIManager::ProcessEvent(SDL_Event event)
{

}

void UIManager::Render(Renderer* renderer)
{
    if(m_activeLayer)
        m_activeLayer->Render(renderer);
}

void UIManager::ActivateLayer(const std::string& name)
{
    if(!m_layers.contains(name))
    {
        SDL_LogError(1, "[UIManager] UI Layer requested does not exist: %s", name.c_str());
        return;
    }

    if(m_activeLayer)
        m_activeLayer->OnDeactivate();
    m_activeLayer = m_layers[name].get();
    m_activeLayer->OnActivate();
}