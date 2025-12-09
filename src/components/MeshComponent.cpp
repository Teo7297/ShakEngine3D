#include "components/MeshComponent.h"
#include "Mesh.h"
#include "Material.h"
#include "Renderer.h"
#include "GameObject.h"
#include "Transform.h"

using namespace Shak;

MeshComponent::MeshComponent(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material)
    :
    m_mesh{ std::move(mesh) },
    m_material{ std::move(material) }
{
}

MeshComponent::~MeshComponent()
{
}

void MeshComponent::Draw(Renderer& renderer)
{
    RenderCommand rc =
    {
        .mesh = m_mesh.get(),
        .material = m_material.get(),
        .modelMatrix = m_transform->GetGlobalMatrix()
    };

    renderer.Submit(rc);
}
