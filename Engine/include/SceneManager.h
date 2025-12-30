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

        template<typename T>
        void RegisterScene(const std::string& name)
        {
            static_assert(std::is_base_of<Scene, T>::value, "T must derive from Scene");
            m_scenes[name] = []() {return std::unique_ptr<T>(new T());}; //? we use this manual new "hack" for portability even though it works with MSVC.
        }

        void LoadScene(const std::string& name);
        void ReloadActiveScene();

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