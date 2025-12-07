#pragma once
#include "GameObject.h"
#include "Shader.h"
#include "Material.h"
#include "Texture.h"
#include "Transform.h"

using namespace Shak;

// static float vertices[] = {
//     // Front face (red)
//     -1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,
//      1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,
//      1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 0.0f,
//     -1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 0.0f,
//     // Back face (green)
//     -1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 0.0f,
//     -1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 0.0f,
//      1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 0.0f,
//      1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 0.0f,
//      // Top face (blue)
//      -1.0f,  1.0f, -1.0f,  0.0f, 0.0f, 1.0f,
//      -1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 1.0f,
//       1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 1.0f,
//       1.0f,  1.0f, -1.0f,  0.0f, 0.0f, 1.0f,
//       // Bottom face (yellow)
//       -1.0f, -1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
//        1.0f, -1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
//        1.0f, -1.0f,  1.0f,  1.0f, 1.0f, 0.0f,
//       -1.0f, -1.0f,  1.0f,  1.0f, 1.0f, 0.0f,
//       // Right face (magenta)
//        1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 1.0f,
//        1.0f,  1.0f, -1.0f,  1.0f, 0.0f, 1.0f,
//        1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
//        1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
//        // Left face (cyan)
//        -1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 1.0f,
//        -1.0f, -1.0f,  1.0f,  0.0f, 1.0f, 1.0f,
//        -1.0f,  1.0f,  1.0f,  0.0f, 1.0f, 1.0f,
//        -1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 1.0f
// };

static float vertices[] = {
    // Front face (red)
    // x, y, z, r, g, b, u, v
    -1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, // Bottom-left
     1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, // Bottom-right
     1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f, // Top-right
    -1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f, // Top-left

    // Back face (green)
    -1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // Bottom-right (relative to front)
    -1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f, // Top-right
     1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f, // Top-left
     1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, // Bottom-left

     // Top face (blue)
     -1.0f,  1.0f, -1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f, // Top-left
     -1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, // Bottom-left
      1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f, // Bottom-right
      1.0f,  1.0f, -1.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f, // Top-right

      // Bottom face (yellow)
      -1.0f, -1.0f, -1.0f,  1.0f, 1.0f, 0.0f,  0.0f, 0.0f, // Bottom-left
       1.0f, -1.0f, -1.0f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f, // Bottom-right
       1.0f, -1.0f,  1.0f,  1.0f, 1.0f, 0.0f,  1.0f, 1.0f, // Top-right
      -1.0f, -1.0f,  1.0f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f, // Top-left

      // Right face (magenta)
       1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 1.0f,  1.0f, 0.0f, // Bottom-right
       1.0f,  1.0f, -1.0f,  1.0f, 0.0f, 1.0f,  1.0f, 1.0f, // Top-right
       1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 1.0f,  0.0f, 1.0f, // Top-left
       1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 1.0f,  0.0f, 0.0f, // Bottom-left

       // Left face (cyan)
       -1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 1.0f,  0.0f, 0.0f, // Bottom-left
       -1.0f, -1.0f,  1.0f,  0.0f, 1.0f, 1.0f,  1.0f, 0.0f, // Bottom-right
       -1.0f,  1.0f,  1.0f,  0.0f, 1.0f, 1.0f,  1.0f, 1.0f, // Top-right
       -1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 1.0f,  0.0f, 1.0f  // Top-left
};

static unsigned int indices[] = {
    0,  1,  2,  2,  3,  0,   // Front
    4,  5,  6,  6,  7,  4,   // Back
    8,  9, 10, 10, 11,  8,   // Top
   12, 13, 14, 14, 15, 12,   // Bottom
   16, 17, 18, 18, 19, 16,   // Right
   20, 21, 22, 22, 23, 20    // Left
};

class TestGO : public Shak::GameObject
{
public:
    TestGO()
        : GameObject()
    {
        m_vertices.assign(vertices, vertices + SDL_arraysize(vertices));
        m_indices.assign(indices, indices + SDL_arraysize(indices));

        auto shader = std::make_shared<Shader>();
        shader->CreateFromBinaryFile(Shader::Type::Vertex, "../shaders/test.vert.spv");
        shader->CreateFromBinaryFile(Shader::Type::Fragment, "../shaders/test.frag.spv");
        shader->Link();

        auto texture = std::make_shared<Texture>();
        texture->LoadFromFile("../../assets/wall.jpg");

        auto material = std::make_shared<Material>();
        material->SetShader(shader);
        material->SetTexture(texture);
        this->SetMaterial(material);

        this->InitDrawBuffers();

        m_transform->Rotate(glm::angleAxis(glm::radians(-55.f), glm::normalize(glm::vec3(1.f, 1.f, 1.f))));
        m_transform->Scale(0.75f * glm::vec3(1.f));
        m_transform->Move(1.f * glm::vec3(0, 1, 0));
        m_transform->SetRotation(m_transform->GetPosition(), m_transform->GetPosition() + glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
    }
};