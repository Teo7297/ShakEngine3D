#include "Transform.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include "glm/gtx/string_cast.hpp"

using namespace Shak;

Transform::Transform()
    :
    m_localMatrix{ glm::identity<glm::mat4>() },
    m_globalMatrix{ glm::identity<glm::mat4>() },
    m_isDirty{ true },
    m_dirtyPosition{ glm::vec3(0.f) },
    m_dirtyRotation{ glm::identity<glm::quat>() },
    m_dirtyScale{ glm::vec3(1.f) },
    m_parent{ nullptr },
    m_children{}
{
}

glm::vec3 Shak::Transform::GetPosition()
{
    this->Decompose();
    return m_translation;
}

glm::quat Shak::Transform::GetRotation()
{
    this->Decompose();
    return m_rotation;
}

glm::vec3 Shak::Transform::GetScale()
{
    this->Decompose();
    return m_scale;
}

void Transform::SetPosition(const glm::vec3& newPosition)
{
    m_dirtyPosition = newPosition;
    this->SetDirtyRecursive();
}

void Shak::Transform::SetRotation(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up)
{
    auto dir = glm::normalize(target - eye);
    m_dirtyRotation = glm::quatLookAt(dir, up);
    this->SetDirtyRecursive();
}

void Shak::Transform::SetRotation(const glm::vec3& direction, const glm::vec3& up)
{
    m_dirtyRotation = glm::quatLookAt(direction, up);
    this->SetDirtyRecursive();
}

void Shak::Transform::SetScale(const glm::vec3& newScale)
{
    m_dirtyScale = newScale;
    this->SetDirtyRecursive();
}

void Shak::Transform::Move(const glm::vec3& offset)
{
    m_dirtyPosition += offset;
    this->SetDirtyRecursive();
}

void Shak::Transform::Rotate(const glm::quat& offset)
{
    m_dirtyRotation = offset * m_dirtyRotation; // reverse order!
    this->SetDirtyRecursive();
}

void Shak::Transform::Scale(const glm::vec3& offset)
{
    m_dirtyScale *= offset;
    this->SetDirtyRecursive();
}

void Transform::SetDirtyRecursive()
{
    if(m_isDirty)
        return;

    m_isDirty = true;
    for(auto* child : m_children)
        child->SetDirtyRecursive();
}

glm::mat4 Shak::Transform::GetLocalMatrix()
{
    return m_localMatrix;
}

glm::mat4 Transform::GetGlobalMatrix()
{
    return m_globalMatrix;
}

void Transform::UpdateMatrices(const glm::mat4& parentGlobalMatrix)
{
    // apply in order scale, rot, transl
    if(m_isDirty)
    {
        // Scale -> Rotate -> Translate
        m_localMatrix = glm::scale(m_localMatrix, m_dirtyScale);
        m_localMatrix = glm::mat4_cast(m_dirtyRotation) * m_localMatrix;
        m_localMatrix = glm::translate(m_localMatrix, m_dirtyPosition);

        // Reset
        m_isDirty = false;
        m_dirtyScale = glm::vec3(1.f);
        m_dirtyRotation = glm::identity<glm::quat>();
        m_dirtyPosition = glm::vec3(0.f);
    }

    m_globalMatrix = parentGlobalMatrix * m_localMatrix;

    for(auto* child : m_children)
        child->UpdateMatrices(m_globalMatrix);
}

void Transform::SetParent(Transform* parent)
{
    m_parent = parent;
}

Transform* Transform::GetParent()
{
    return m_parent;
}

void Transform::AddChild(Transform* child)
{
    m_children.emplace_back(child);
    child->SetParent(this);
}

void Transform::PrintGlobalMatrix()
{
    SDL_Log(glm::to_string(this->GetGlobalMatrix()).c_str());
}

void Transform::PrintLocalMatrix()
{
    SDL_Log(glm::to_string(this->GetLocalMatrix()).c_str());
}

void Shak::Transform::PrintMatrix(const glm::mat4& mat)
{
    SDL_Log(glm::to_string(mat).c_str());
}

void Shak::Transform::Decompose()
{
    if(!m_isDirty)
        return;

    glm::decompose(this->GetLocalMatrix(), m_scale, m_rotation, m_translation, m_skew, m_perspective);
}
