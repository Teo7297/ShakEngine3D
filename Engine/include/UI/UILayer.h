#pragma once

#include "Includes.h"
#include "UI/UIWidget.h"

namespace Shak
{
    class Renderer;
    class UILayer
    {
        friend class UIManager;
    public:
        virtual ~UILayer();

        virtual void Initialize();
        virtual void OnActivate();
        virtual void OnDeactivate();
        virtual void Update(float dt);
        
        void Render(Renderer* renderer);

        void SetName(const std::string& name);

        template<typename T>
        T* AddWidget(const std::string& name)
        {
            static_assert(std::is_base_of<UIWidget, T>::value, "T must be a UIWidget");

            auto widget = std::unique_ptr<T>(new T());
            widget->SetName(name);
            widget->Initialize();
            T* rawWidget = (T*)widget.get();
            m_widgets[name] = std::move(widget);
            return rawWidget;
        }

    protected:
        UILayer();

    protected:
        std::string m_name;
        std::unordered_map<std::string, std::unique_ptr<UIWidget>> m_widgets;
    };
}