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

void MeshComponent::SetMesh(Mesh* mesh)
{
    m_mesh = mesh;
}


void MeshComponent::SetMaterial(Material* material)
{
    m_material = material;
}

void MeshComponent::Draw(Renderer* renderer)
{
    renderer->Submit({
        .mesh = m_mesh,
        .material = m_material,
        .modelMatrix = m_transform->GetGlobalMatrix(),
        .isSkybox = m_isSkybox,
        .renderMode = GL_TRIANGLES
        });
}
