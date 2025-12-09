#include "Mesh.h"

using namespace Shak;

Mesh::Mesh(std::vector<float> vertices, std::vector<GLuint> indices)
    :
    m_vertices{ std::move(vertices) },
    m_indices{ std::move(indices) }
{
    InitGLBuffers();
}

Mesh::~Mesh()
{
    if(m_vao) {
        GL_CHECK(glDeleteVertexArrays(1, &m_vao));
        m_vao = 0u;
    }
    if(m_vbo) {
        GL_CHECK(glDeleteBuffers(1, &m_vbo));
        m_vbo = 0u;
    }
    if(m_ebo) {
        GL_CHECK(glDeleteBuffers(1, &m_ebo));
        m_ebo = 0u;
    }
}

void Mesh::Bind()
{
    GL_CHECK(glBindVertexArray(m_vao));
}

int Mesh::GetIndicesCount()
{
    return m_indices.size();
}

void Mesh::InitGLBuffers()
{
    GL_CHECK(glGenVertexArrays(1, &m_vao));
    GL_CHECK(glGenBuffers(1, &m_vbo));
    GL_CHECK(glGenBuffers(1, &m_ebo));

    GL_CHECK(glBindVertexArray(m_vao));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), m_vertices.data(), GL_STATIC_DRAW));

    // Position attribute
    GLuint attribCount = 0;
    GL_CHECK(glVertexAttribPointer(
        attribCount,
        3,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void*)0
    ));
    GL_CHECK(glEnableVertexAttribArray(attribCount++));

    // Color attribute
    GL_CHECK(glVertexAttribPointer(
        attribCount,
        3,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void*)(3 * sizeof(float))
    ));
    GL_CHECK(glEnableVertexAttribArray(attribCount++));

    // Texture coordinates
    GL_CHECK(glVertexAttribPointer(
        attribCount,
        2,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void*)(6 * sizeof(float))
    ));
    GL_CHECK(glEnableVertexAttribArray(attribCount++));

    // EBO, pass in the indices data
    GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo));
    GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), m_indices.data(), GL_STATIC_DRAW));

    // Unbind
    GL_CHECK(glBindVertexArray(0));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}