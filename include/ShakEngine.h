#pragma once

#include "Includes.h"
#include "interfaces/IAppControl.h"

namespace Shak
{
    struct AppMetadata
    {
        const char* key;
        const char* value;
    };

    class AssetManager;
    class SceneManager;
    class UIManager;
    class Renderer;
    class ShakEngine : public IAppControl
    {
    public:
        ShakEngine();
        ~ShakEngine();

        void Init(const std::vector<AppMetadata>& metadata, const char* windowName, int winWidth, int winHeight, bool resizable, int syncType);
        void Start();
        void Stop() override;
        void SetResolution(int w, int h) override;

        AppContext& GetAppContext();

    private:
        void Update();
        void InitSDL(const std::vector<AppMetadata>& metadata, const char* windowName, int winWidth, int winHeight, bool resizable, int syncType);
        void InitGlew();
        void InitImGui();
        void LogGLVersion();
        void FreeResources();
        float GetDeltaTime();

    private:
        SDL_Window* m_window;
        SDL_GLContext m_GLContext;
        std::unique_ptr<AssetManager> m_assetManager;
        std::unique_ptr<SceneManager> m_sceneManager;
        std::unique_ptr<UIManager> m_uiManager;
        std::unique_ptr<Renderer> m_renderer;
        bool m_shouldStop;
        AppContext m_appContext;
    };
}