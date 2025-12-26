#include "SceneManager.h"
#include "Renderer.h"
#include "AssetManager.h"

using namespace Shak;

SceneManager::SceneManager()
    : m_scenes{},
    m_sceneLoadRequested{ false },
    m_sceneToLoad{ "" }
{

}

Scene* SceneManager::GetActiveScene()
{
    return m_activeScene.get();
}

void SceneManager::LoadScene(const std::string& name)
{
    m_sceneLoadRequested = true;
    m_sceneToLoad = name;
}

void SceneManager::Update(Renderer* renderer, float deltaTime)
{
    // First of all, check if a scene needs to be loaded
    if(m_sceneLoadRequested)
        this->StartSceneLoading();

    m_activeScene->Update(renderer, deltaTime);
}

void SceneManager::ProcessEvent(SDL_Event event)
{
    m_activeScene->ProcessEvent(event);
}

void SceneManager::StartSceneLoading()
{
    auto it = m_scenes.find(m_sceneToLoad);
    if(it != m_scenes.end())
    {
        m_activeScene.reset();
        m_appContext.assetManager->ClearSceneData();

        m_activeScene = it->second();

        m_activeScene->SetAppContext(m_appContext);
        m_activeScene->Init();
    }
    m_sceneLoadRequested = false;
}