#pragma once

#include "Includes.h"

namespace Shak
{
    class Mesh
    {
    public:
        // Data should be passed with std::move in here
        Mesh(std::vector<float> vertices, std::vector<GLuint> indices, bool hasColors, bool hasTextureCoords);
        ~Mesh();
        void Bind();
        int GetIndicesCount();

    private:
        void InitGLBuffers();

    private:
        std::vector<float> m_vertices;
        std::vector<GLuint> m_indices;
        GLuint m_vao, m_vbo, m_ebo;
        bool m_hasColors, m_hasTextureCoords;
    };
}