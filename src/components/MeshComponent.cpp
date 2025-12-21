#include "components/MeshComponent.h"
#include "Mesh.h"
#include "Material.h"
#include "Renderer.h"
#include "GameObject.h"
#include "Transform.h"

using namespace Shak;

MeshComponent::MeshComponent()
    : m_isSkybox{ false }
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
        .modelMatrix = m_transform->GetGlobalMatrix(),
        .isSkybox = m_isSkybox,
        .renderMode = GL_TRIANGLES
    };

    renderer.Submit(rc);
}
