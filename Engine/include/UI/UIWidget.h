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

        void SetActive(bool flag);
        bool IsActive();

        void Draw(Renderer* renderer) override;

    protected:
        UIWidget();
        virtual void OnActivate();
        virtual void OnDeactivate();

    protected:
        bool m_active = false;
        Mesh* m_mesh;
        Material* m_material;
        glm::vec2 m_screenCoords;
    };
}