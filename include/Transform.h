#pragma once

#include "Includes.h"

namespace Shak
{
    class Transform
    {
    public:
        Transform();
        ~Transform() = default;

        glm::vec3 GetPosition();
        glm::quat GetRotation();
        glm::vec3 GetScale();

        void SetPosition(const glm::vec3& newPosition);
        void SetRotation(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up);
        void SetRotation(const glm::vec3& direction, const glm::vec3& up);
        void SetScale(const glm::vec3& newScale);

        void Move(const glm::vec3& offset);
        void Rotate(const glm::quat& offset);
        void Scale(const glm::vec3& offset);
        
        void SetDirtyRecursive();

        glm::mat4 GetLocalMatrix();
        glm::mat4 GetGlobalMatrix();
        void UpdateMatrices(const glm::mat4& parentGlobalMatrix);

        void SetParent(Transform* parent);
        Transform* GetParent();

        void AddChild(Transform* child);
        std::vector<Transform*> GetChildren();

        //DEBUG
        void PrintGlobalMatrix();
        void PrintLocalMatrix();
        static void PrintMatrix(const glm::mat4& mat);

    private:
        void Decompose();

    private:
        glm::mat4 m_localMatrix, m_globalMatrix;

        bool m_isDirty;
        glm::vec3 m_dirtyPosition;
        glm::quat m_dirtyRotation;
        glm::vec3  m_dirtyScale;

        glm::vec3 m_scale;
        glm::quat m_rotation;
        glm::vec3 m_translation;
        glm::vec3 m_skew;
        glm::vec4 m_perspective;

        // Hierarchy
        Transform* m_parent;
        std::vector<Transform*> m_children;
    };
}