#pragma once

#include "Includes.h"

namespace Shak
{
    class GameObject
    {
    public:
        GameObject(const std::vector<float>& vertices, const std::vector<GLuint>& indices);
        ~GameObject();

        void SetShader(const std::shared_ptr<class Shader>& shader);

        void Draw();

    private:
        std::shared_ptr<class Transform> m_transform;
        std::vector<float> m_vertices;
        std::vector<GLuint> m_indices;
        GLuint m_vao, m_vbo, m_ebo;
        std::shared_ptr<class Shader> m_shader;
    };
}