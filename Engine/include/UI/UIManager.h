#pragma once

#include "Includes.h"
#include "UI/UILayer.h"

namespace Shak
{
    class Renderer;
    class UIManager
    {
    public:
        void Update(float deltaTime);
        void ProcessEvent(SDL_Event event);
        void Render(Renderer* renderer);

        template<typename T>
        T* CreateUILayer(const std::string& name)
        {
            static_assert(std::is_base_of<UILayer, T>::value, "T must be a UILayer");

            auto layer = std::unique_ptr<T>(new T());
            layer->SetName(name);
            layer->Initialize();
            T* rawLayer = (T*)layer.get();
            m_layers[name] = std::move(layer);
            return rawLayer;
        }

        void ActivateLayer(const std::string& name);

    private:
        UILayer* m_activeLayer = nullptr;
        std::unordered_map<std::string, std::unique_ptr<UILayer>> m_layers;
    };
}