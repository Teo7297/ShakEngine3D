#include "components/MeshComponent.h"
#include "Mesh.h"
#include "Material.h"
#include "Renderer.h"
#include "GameObject.h"
#include "Transform.h"

using namespace Shak;

MeshComponent::MeshComponent()
{
}

MeshComponent::~MeshComponent()
{
}

void MeshComponent::SetMesh(std::shared_ptr<Mesh> mesh)
{
    m_mesh = mesh;
}


void MeshComponent::SetMaterial(std::shared_ptr<Material> material)
{
    m_material = material;
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
