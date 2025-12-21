#pragma once

#include "Includes.h"


namespace Shak
{
    class Transform
    {
        friend class GameObject;
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

        glm::mat4 GetLocalMatrix() const;
        glm::mat4 GetGlobalMatrix() const;
        void UpdateMatrices(const glm::mat4& parentGlobalMatrix);

        glm::vec3 Front();
        glm::vec3 Right();
        glm::vec3 Up();

        void SetParent(Transform* parent);
        Transform* GetParent() const;

        void AddChild(Transform* child);
        std::vector<Transform*> GetChildren() const;

        GameObject* GetGameObject() const;

        //DEBUG
        void PrintGlobalMatrix();
        void PrintLocalMatrix();
        static void PrintMatrix(const glm::mat4& mat);

    private:
        void Decompose();
        void UpdateVectors();

    private:
        glm::mat4 m_localMatrix, m_globalMatrix;

        bool m_isDirty;
        glm::vec3 m_dirtyPosition;
        glm::quat m_dirtyRotation;
        glm::vec3  m_dirtyScale;

        bool m_localMatrixUpdated;
        glm::vec3 m_scale;
        glm::quat m_rotation;
        glm::vec3 m_translation;
        glm::vec3 m_skew;
        glm::vec4 m_perspective;
        glm::vec3 m_front;
        glm::vec3 m_right;
        glm::vec3 m_up;

        // Hierarchy
        Transform* m_parent;
        std::vector<Transform*> m_children;
        GameObject* m_ownerGameObject;
    };
}