#pragma once

#include "Includes.h"
#include "Component.h"
#include "interfaces/IRenderable.h"

namespace Shak
{
    class Mesh;
    class Material;
    class Renderer;
    class MeshComponent : public Component, public IRenderable
    {
    public:
        MeshComponent();
        ~MeshComponent() override;

        void SetMesh(Mesh* mesh);
        void SetMaterial(Material* material);
        void SetIsSkybox(bool flag) { m_isSkybox = flag; } // TODO: maybe we can put this in another IRenderable 

        void Draw(Renderer* renderer) override;
        
    private:
        Mesh* m_mesh;
        Material* m_material;
        bool m_isSkybox;
    };
}