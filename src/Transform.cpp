#include "Transform.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/quaternion.hpp>

using namespace Shak;

Transform::Transform()
    :
    m_localMatrix{ glm::identity<glm::mat4>() },
    m_globalMatrix{ glm::identity<glm::mat4>() },
    m_isDirty{ true },
    m_dirtyPosition{ glm::vec3(0.f) },
    m_dirtyRotation{ glm::identity<glm::quat>() },
    m_dirtyScale{ glm::vec3(1.f) },
    m_localMatrixUpdated{ true },
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

void Transform::SetRotation(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up)
{
    auto dir = target - eye;
    auto dirLen = glm::length(dir);
    if(dirLen < 0.0001f) return;

    auto dirNormalized = dir * (1.f / dirLen);

    this->SetRotation(dirNormalized, up);
}

void Transform::SetRotation(const glm::vec3& direction, const glm::vec3& up)
{
    // 1. Define the Basis Vectors
    // Normalize the target direction
    glm::vec3 zAxis = -direction; // Align Local -Z (Forward) with direction

    // Calculate Right (X) and Up (Y)
    glm::vec3 xAxis = glm::normalize(glm::cross(up, zAxis));
    glm::vec3 yAxis = glm::cross(zAxis, xAxis);

    // 2. Update the rotation part of the matrix directly
    // GLM is column-major: mat4[col][row]

    // Column 0: Right (X)
    m_localMatrix[0][0] = xAxis.x;
    m_localMatrix[0][1] = xAxis.y;
    m_localMatrix[0][2] = xAxis.z;

    // Column 1: Up (Y)
    m_localMatrix[1][0] = yAxis.x;
    m_localMatrix[1][1] = yAxis.y;
    m_localMatrix[1][2] = yAxis.z;

    // Column 2: Backward (Z)
    m_localMatrix[2][0] = zAxis.x;
    m_localMatrix[2][1] = zAxis.y;
    m_localMatrix[2][2] = zAxis.z;

    // Column 3 (Position) is left untouched

    // 3. Clear pending dirty rotation to avoid double application
    m_dirtyRotation = glm::identity<glm::quat>();

    m_localMatrixUpdated = true;
}

void Transform::SetScale(const glm::vec3& newScale)
{
    m_dirtyScale = newScale;
    this->SetDirtyRecursive();
}

void Transform::Move(const glm::vec3& offset)
{
    m_dirtyPosition += offset;
    this->SetDirtyRecursive();
}

void Transform::Rotate(const glm::quat& offset)
{
    m_dirtyRotation = offset * m_dirtyRotation; // reverse order!
    this->SetDirtyRecursive();
}

void Transform::Scale(const glm::vec3& offset)
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

glm::mat4 Shak::Transform::GetLocalMatrix() const
{
    return m_localMatrix;
}

glm::mat4 Transform::GetGlobalMatrix() const
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
        m_localMatrix = m_localMatrix * glm::mat4_cast(m_dirtyRotation);
        m_localMatrix = glm::translate(m_localMatrix, m_dirtyPosition);

        // Reset
        m_localMatrixUpdated = true;
        m_isDirty = false;
        m_dirtyScale = glm::vec3(1.f);
        m_dirtyRotation = glm::identity<glm::quat>();
        m_dirtyPosition = glm::vec3(0.f);
    }

    m_globalMatrix = parentGlobalMatrix * m_localMatrix;

    for(auto* child : m_children)
        child->UpdateMatrices(m_globalMatrix);
}

glm::vec3 Transform::Front()
{
    this->Decompose();
    return m_front;
}

glm::vec3 Transform::Right()
{
    this->Decompose();
    return m_right;
}

glm::vec3 Transform::Up()
{
    this->Decompose();
    return m_up;
}

void Transform::SetParent(Transform* parent)
{
    m_parent = parent;
}

Transform* Transform::GetParent() const
{
    return m_parent;
}

void Transform::AddChild(Transform* child)
{
    m_children.emplace_back(child);
    child->SetParent(this);
}

std::vector<Transform*> Transform::GetChildren() const
{
    return m_children;
}

GameObject* Transform::GetGameObject() const
{
    if(m_ownerGameObject)
        return m_ownerGameObject;
    return nullptr;
}

void Transform::PrintGlobalMatrix()
{
    SDL_Log(glm::to_string(this->GetGlobalMatrix()).c_str());
}

void Transform::PrintLocalMatrix()
{
    SDL_Log(glm::to_string(this->GetLocalMatrix()).c_str());
}

void Transform::PrintMatrix(const glm::mat4& mat)
{
    SDL_Log(glm::to_string(mat).c_str());
}

void Transform::Decompose()
{
    if(!m_localMatrixUpdated)
        return;

    glm::decompose(m_localMatrix, m_scale, m_rotation, m_translation, m_skew, m_perspective);

    this->UpdateVectors();

    m_localMatrixUpdated = false;
}

void Transform::UpdateVectors()
{
    m_front = m_rotation * glm::vec3(0, 0, -1);
    m_right = m_rotation * glm::vec3(1, 0, 0);
    m_up = m_rotation * glm::vec3(0, 1, 0);
}
