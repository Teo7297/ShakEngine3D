#include "GameObject.h"
#include "Transform.h"
#include "Shader.h"

using namespace Shak;


GameObject::GameObject(const std::vector<float>& vertices, const std::vector<GLuint>& indices)
    :
    m_vertices{ vertices },
    m_indices{ indices },
    m_transform{ std::make_shared<Transform>() },
    m_shader{ nullptr }
{
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), m_vertices.data(), GL_STATIC_DRAW);

    // Position attribute
    GLuint attribCount = 0;
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float),
        (void*)0
    );
    glEnableVertexAttribArray(attribCount++);

    // Color attribute
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float),
        (void*)(3 * sizeof(float))
    );
    glEnableVertexAttribArray(attribCount++);

    // EBO, pass in the indices data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), m_indices.data(), GL_STATIC_DRAW);

    // Unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GameObject::~GameObject()
{
    if(m_vao) {
        glDeleteVertexArrays(1, &m_vao);
        m_vao = 0u;
    }
    if(m_vbo) {
        glDeleteBuffers(1, &m_vbo);
        m_vbo = 0u;
    }
    if(m_ebo) {
        glDeleteBuffers(1, &m_ebo);
        m_ebo = 0u;
    }
}

void Shak::GameObject::SetShader(const std::shared_ptr<class Shader>& shader)
{
    m_shader = shader;
}


void GameObject::Draw()
{
    if(m_shader)
        m_shader->Bind();
    GL_CHECK(glBindVertexArray(m_vao));
    GL_CHECK(glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0));
    GL_CHECK(glBindVertexArray(0));
}

