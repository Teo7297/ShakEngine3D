#pragma once

#include "Includes.h"

namespace Shak
{
    class Camera
    {
    public:
        Camera();
        ~Camera() = default;
        glm::mat4 GetViewProjectionMatrix();

    private:
        std::shared_ptr<class Transform> m_transform;
        glm::mat4 m_projection;
    };
}