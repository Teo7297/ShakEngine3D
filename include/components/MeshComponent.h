#pragma once

#include "Includes.h"
#include "Component.h"

namespace Shak
{
    class Mesh;
    class Material;
    class Renderer;
    class MeshComponent : public Component
    {
    public:
        //TODO: use raw ptrs here. leave ownership to resource manager!
        MeshComponent();
        ~MeshComponent() override;

        void SetMesh(std::shared_ptr<Mesh> mesh);
        void SetMaterial(std::shared_ptr<Material> material);

        void Draw(Renderer& renderer);
    private:
        std::shared_ptr<Mesh> m_mesh;
        std::shared_ptr<Material> m_material;
    };
}