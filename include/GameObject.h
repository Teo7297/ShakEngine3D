#pragma once

#include "Includes.h"

namespace Shak
{
    class GameObject
    {
    public:
        GameObject();
        ~GameObject();

        void SetMaterial(const std::shared_ptr<class Material>& materila);

        void Draw();

    protected:
        void InitDrawBuffers();

    protected:
        std::shared_ptr<class Transform> m_transform;
        std::vector<float> m_vertices;
        std::vector<GLuint> m_indices;
        GLuint m_vao, m_vbo, m_ebo;
        std::shared_ptr<class Material> m_material;
    };
}