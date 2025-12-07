#include "GameObject.h"
#include "Transform.h"
#include "Material.h"
#include "Shader.h"

using namespace Shak;
struct MatrixBlock {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
};

GameObject::GameObject()
    :
    m_transform{ std::make_shared<Transform>() },
    m_material{ nullptr }
{
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

void Shak::GameObject::SetMaterial(const std::shared_ptr<Material>& material)
{
    m_material = material;
}

void GameObject::Draw()
{
    auto M = m_transform->GetUpdatedMatrix();

    auto V = glm::identity<glm::mat4>();
    V = glm::translate(V, glm::vec3(0.f, 0.f, -6.f));

    auto P = glm::perspective(glm::radians(45.0f), 800.f / 600.f, 0.1f, 100.0f);

    MatrixBlock mvp = { .model = M, .view = V, .projection = P };

    auto shader = m_material->GetShader();
    shader->SetMVP(mvp);
    shader->SetUniformFloat(0, (float)SDL_GetTicks());

    if(m_material)
        m_material->Bind();
    GL_CHECK(glBindVertexArray(m_vao));
    GL_CHECK(glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0));
    GL_CHECK(glBindVertexArray(0));
}

void GameObject::InitDrawBuffers()
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
        attribCount,
        3,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void*)0
    );
    glEnableVertexAttribArray(attribCount++);

    // Color attribute
    glVertexAttribPointer(
        attribCount,
        3,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void*)(3 * sizeof(float))
    );
    glEnableVertexAttribArray(attribCount++);

    glVertexAttribPointer(
        attribCount,
        2,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void*)(6 * sizeof(float))
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

