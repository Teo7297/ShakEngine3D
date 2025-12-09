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
        MeshComponent(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material);
        ~MeshComponent() override;

        void Draw(Renderer& renderer);
    private:
        std::shared_ptr<Mesh> m_mesh;
        std::shared_ptr<Material> m_material;
    };
}