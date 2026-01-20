#pragma once

#include "Component.h"
#include "GameObject.h"
#include "Transform.h"

namespace Shak
{
    class CameraComponent : public Component {
    private:
        // Projection Settings
        glm::mat4 m_projectionMatrix;
        float m_fov = 45.0f;
        float m_aspectRatio = 1920.f / 1080.f;
        float m_nearClip = 0.1f;
        float m_farClip = 100.0f;

    public:
        void Init(float fov = 45.f, float aspectRatio = 1920.f / 1080.f, float nearClip = 0.1f, float farClip = 100.f)
        {
            m_fov = fov;
            m_aspectRatio = aspectRatio;
            m_nearClip = nearClip;
            m_farClip = farClip;
        }

        void OnAwake() override {
            RecalculateProjection();
        }

        void SetPerspective(float fov, float aspect, float nearClip, float farClip) {
            m_fov = fov;
            m_aspectRatio = aspect;
            m_nearClip = nearClip;
            m_farClip = farClip;
            RecalculateProjection();
        }

        // Called when window resizes
        void SetViewportSize(uint32_t width, uint32_t height) {
            m_aspectRatio = (float)width / (float)height;
            RecalculateProjection();
        }

        void RecalculateProjection() {
            m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearClip, m_farClip);
        }

        const glm::mat4& GetProjectionMatrix() const {
            return m_projectionMatrix;
        }

        // CRITICAL: The View Matrix is the INVERSE of the Transform Matrix
        // If the camera moves Right (World +X), the world moves Left (View -X).
        glm::mat4 GetViewMatrix() const {
            // We use the Transform we are attached to!
            return glm::inverse(m_owner->GetTransform()->GetGlobalMatrix());
        }

        // Helper for the Renderer
        glm::mat4 GetViewProjectionMatrix() const {
            return m_projectionMatrix * GetViewMatrix();
        }
    };
}