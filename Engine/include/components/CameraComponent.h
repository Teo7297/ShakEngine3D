#pragma once

#include "Component.h"
#include "GameObject.h"
#include "Transform.h"

namespace Shak
{
    class CameraComponent : public Component
    {
    public:
        void Init(float fov = 45.f, int width = 1920, int height = 1080, float nearClip = 0.1f, float farClip = 100.f)
        {
            m_fov = fov;
            m_aspectRatio = width / height;
            m_width = width;
            m_height = height;
            m_nearClip = nearClip;
            m_farClip = farClip;
        }

        void OnAwake() override
        {
            RecalculateProjection();
        }

        void SetPerspective(float fov, int width, int height, float nearClip, float farClip) {
            m_fov = fov;
            m_aspectRatio = width / height;
            m_width = width;
            m_height = height;
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
            m_UIProjectionMatrix = glm::ortho(0, m_width, m_height, 0, -1, 1);
        }

        const glm::mat4& GetProjectionMatrix() const {
            return m_projectionMatrix;
        }

        const glm::mat4& GetUIProjectionMatrix() const {
            return m_UIProjectionMatrix;
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

    private:
        // Projection Settings
        glm::mat4 m_projectionMatrix;
        glm::mat4 m_UIProjectionMatrix;
        int m_width = 1920;
        int m_height = 1080;
        float m_fov = 45.0f;
        float m_aspectRatio = 1920.f / 1080.f;
        float m_nearClip = 0.1f;
        float m_farClip = 100.0f;
    };


}