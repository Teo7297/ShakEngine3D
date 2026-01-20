#pragma once

struct ImGuiContext;
namespace Shak
{
    class AssetManager;
    class SceneManager;
    class UIManager;
    class Renderer;
    class IAppControl;
    struct AppContext
    {
        SDL_Window* window;
        AssetManager* assetManager;
        SceneManager* sceneManager;
        UIManager* uiManager;
        Renderer* renderer;
        IAppControl* appControl;
        SDL_GLContext* glContext;
        ImGuiContext* imguiContext; //! This still points to some static variables!
    };
}