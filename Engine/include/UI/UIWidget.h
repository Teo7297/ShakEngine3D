#pragma once

#include "Includes.h"
#include "interfaces/IRenderable.h"

namespace Shak
{
    class Mesh;
    class Material;
    class UIWidget : public IRenderable
    {
        friend class UILayer;
    public:
        virtual ~UIWidget();

        virtual void Initialize();
        virtual void Update(float dt);
        virtual void ProcessEvent(SDL_Event event);

        void SetActive(bool flag);
        bool IsActive();

        void SetName(const std::string& name);
        std::string GetName();

        void Draw(Renderer* renderer) override;

    protected:
        UIWidget();
        virtual void OnActivate();
        virtual void OnDeactivate();

    protected:
        std::string m_name = "";
        bool m_active = false;
        Mesh* m_mesh;
        Material* m_material;
        glm::vec2 m_screenCoords;
    };
}