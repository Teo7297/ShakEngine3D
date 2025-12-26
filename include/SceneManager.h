#pragma once

#include "Includes.h"
#include "Scene.h"
#include "AppContext.h"

namespace Shak
{
    class Renderer;
    class SceneManager
    {
    public:
        using SceneFactory = std::function<std::unique_ptr<Scene>()>;

        SceneManager();
        ~SceneManager() = default;

        void RegisterScene(const std::string& name, SceneFactory factory)
        {
            m_scenes[name] = factory;
        }

        void LoadScene(const std::string& name);

        Scene* GetActiveScene();

        void SetAppContext(AppContext& ctx) { m_appContext = ctx; }

        void Update(Renderer* renderer, float deltaTime);
        void ProcessEvent(SDL_Event event);

    private:
        void StartSceneLoading();

    private:
        std::unordered_map<std::string, SceneFactory> m_scenes;
        std::unique_ptr<Scene> m_activeScene;
        AppContext m_appContext;
        bool m_sceneLoadRequested;
        std::string m_sceneToLoad;
    };
}