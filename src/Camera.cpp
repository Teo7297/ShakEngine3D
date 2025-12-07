#include "Camera.h"

#include "Transform.h"

using namespace Shak;

Camera::Camera()
    : m_projection{ glm::perspective(glm::radians(45.0f), 800.f / 600.f, 0.1f, 100.0f) }
{
}

glm::mat4 Camera::GetViewProjectionMatrix()
{
    return m_projection * glm::inverse(m_transform->GetUpdatedMatrix());
}
