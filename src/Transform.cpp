#include "Transform.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

using namespace Shak;

Transform::Transform()
    :
    m_matrix{ glm::identity<glm::mat4>() },
    m_isDirty{ true },
    m_dirtyPosition{ glm::vec3(0.f) },
    m_dirtyRotation{ glm::identity<glm::quat>() },
    m_dirtyScale{ glm::vec3(1.f) }
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
    m_isDirty = true;
}

void Shak::Transform::SetRotation(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up)
{
    auto dir = glm::normalize(target - eye);
    m_dirtyRotation = glm::quatLookAt(dir, up);
    m_isDirty = true;
}

void Shak::Transform::SetRotation(const glm::vec3& direction, const glm::vec3& up)
{
    m_dirtyRotation = glm::quatLookAt(direction, up);
    m_isDirty = true;
}

void Shak::Transform::SetScale(const glm::vec3& newScale)
{
    m_dirtyScale = newScale;
    m_isDirty = true;
}

void Shak::Transform::Move(const glm::vec3& offset)
{
    m_dirtyPosition += offset;
    m_isDirty = true;
}

void Shak::Transform::Rotate(const glm::quat& offset)
{
    m_dirtyRotation = offset * m_dirtyRotation; // reverse order!
    m_isDirty = true;
}

void Shak::Transform::Scale(const glm::vec3& offset)
{
    m_dirtyScale *= offset;
    m_isDirty = true;
}

glm::mat4 Shak::Transform::GetUpdatedMatrix()
{
    // apply in order scale, rot, transl
    if(m_isDirty)
    {
        // Scale -> Rotate -> Translate
        m_matrix = glm::scale(m_matrix, m_dirtyScale);
        m_matrix = glm::mat4_cast(m_dirtyRotation) * m_matrix;
        m_matrix = glm::translate(m_matrix, m_dirtyPosition);

        // Reset
        m_isDirty = false;
        m_dirtyScale = glm::vec3(1.f);
        m_dirtyRotation = glm::identity<glm::quat>();
        m_dirtyPosition = glm::vec3(0.f);
    }

    return m_matrix;
}

void Shak::Transform::Decompose()
{
    if(!m_isDirty)
        return;

    glm::decompose(this->GetUpdatedMatrix(), m_scale, m_rotation, m_translation, m_skew, m_perspective);
}
