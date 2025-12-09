#pragma once

#include "Includes.h"
#include "GameObject.h"
#include "Mesh.h"
#include "components/MeshComponent.h"
#include "Material.h"
#include "Shader.h"
#include "Texture.h"

using namespace Shak;

/* Cube vertices with colors (position XYZ, color RGB) */
static std::vector<float> vertices = {
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

static std::vector<unsigned int> indices = {
    0,  1,  2,  2,  3,  0,   // Front
    4,  5,  6,  6,  7,  4,   // Back
    8,  9, 10, 10, 11,  8,   // Top
   12, 13, 14, 14, 15, 12,   // Bottom
   16, 17, 18, 18, 19, 16,   // Right
   20, 21, 22, 22, 23, 20    // Left
};

class TestCube : public GameObject
{
public:
    TestCube() { m_active = true; }
    ~TestCube() override {}

    void OnAwake() override
    {
        auto mesh = std::make_shared<Mesh>(vertices, indices);

        auto shader = std::make_shared<Shader>();
        shader->CreateFromBinaryFile(Shader::Type::Vertex, "../shaders/test.vert.spv");
        shader->CreateFromBinaryFile(Shader::Type::Fragment, "../shaders/test.frag.spv");
        shader->Link();

        auto texture = std::make_shared<Texture>();
        texture->LoadFromFile("../../assets/wall.jpg", GL_RGB);

        auto material = std::make_shared<Material>();
        material->SetShader(shader);
        material->AddTexture(texture);

        meshComp = (MeshComponent*)this->AddComponent<MeshComponent>();
        meshComp->SetMesh(std::move(mesh));
        meshComp->SetMaterial(std::move(material));
    }

    void OnUpdate(float dt) override
    {
        const bool* key_states = SDL_GetKeyboardState(NULL);
        static float speed = 2.f;
        if(key_states[SDL_SCANCODE_L])
            this->GetTransform()->Move(glm::vec3(1, 0, 0) * speed * dt);
        if(key_states[SDL_SCANCODE_J])
            this->GetTransform()->Move(glm::vec3(-1, 0, 0) * speed * dt);
        if(key_states[SDL_SCANCODE_I])
            this->GetTransform()->Move(glm::vec3(0, 1, 0) * speed * dt);
        if(key_states[SDL_SCANCODE_K])
            this->GetTransform()->Move(glm::vec3(0, -1, 0) * speed * dt);
    }

    MeshComponent* meshComp; // not here
};