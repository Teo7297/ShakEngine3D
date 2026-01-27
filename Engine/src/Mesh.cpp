#include "Mesh.h"

using namespace Shak;

Mesh::Mesh(std::vector<float> vertices, std::vector<GLuint> indices, bool hasColors, bool hasTextureCoords)
    :
    m_vertices{ std::move(vertices) },
    m_indices{ std::move(indices) },
    m_hasColors{ hasColors },
    m_hasTextureCoords{ hasTextureCoords }
{
    InitGLBuffers();
}

Mesh::~Mesh()
{
    this->ClearGLResources();
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
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), m_vertices.data(), GL_DYNAMIC_DRAW));

    GLsizei stride = (3 + (m_hasColors ? 3 : 0) + (m_hasTextureCoords ? 2 : 0)) * sizeof(float);
    size_t offset = 0;

    // Position attribute
    GLuint attribCount = 0;
    GL_CHECK(glVertexAttribPointer(
        attribCount,
        3,
        GL_FLOAT,
        GL_FALSE,
        stride,
        (void*)offset
    ));
    GL_CHECK(glEnableVertexAttribArray(attribCount++));
    offset += 3 * sizeof(float);

    // Color attribute
    if(m_hasColors)
    {
        GL_CHECK(glVertexAttribPointer(
            attribCount,
            3,
            GL_FLOAT,
            GL_FALSE,
            stride,
            (void*)offset
        ));
        GL_CHECK(glEnableVertexAttribArray(attribCount++));
        offset += 3 * sizeof(float);
    }

    // Texture coordinates
    if(m_hasTextureCoords)
    {
        GL_CHECK(glVertexAttribPointer(
            attribCount,
            2,
            GL_FLOAT,
            GL_FALSE,
            stride,
            (void*)offset
        ));
        GL_CHECK(glEnableVertexAttribArray(attribCount++));
        offset += 2 * sizeof(float);
    }

    // EBO, pass in the indices data
    GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo));
    GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), m_indices.data(), GL_DYNAMIC_DRAW));

    // Unbind
    GL_CHECK(glBindVertexArray(0));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void Mesh::ClearGLResources()
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
