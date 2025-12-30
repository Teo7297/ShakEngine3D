#pragma once

namespace Shak
{
    class AssetManager;
    class SceneManager;
    class UIManager;
    class Renderer;
    class IAppControl;
    struct AppContext
    {
        AssetManager* assetManager;
        SceneManager* sceneManager;
        UIManager* uiManager;
        Renderer* renderer;
        IAppControl* appControl;
    };
}