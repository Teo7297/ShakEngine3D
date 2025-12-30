#pragma once

#include "Includes.h"
#include "GameObject.h"
#include "Mesh.h"
#include "components/MeshComponent.h"
#include "Material.h"
#include "Shader.h"
#include "Texture.h"
#include "components/TestComponent.h"
#include "AssetManager.h"

using namespace Shak;
class TestCube : public GameObject
{
    /* Cube vertices with colors (position XYZ, color RGB) */
    std::vector<float> vertices = {
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

    std::vector<unsigned int> indices = {
        0,  1,  2,  2,  3,  0,   // Front
        4,  5,  6,  6,  7,  4,   // Back
        8,  9, 10, 10, 11,  8,   // Top
       12, 13, 14, 14, 15, 12,   // Bottom
       16, 17, 18, 18, 19, 16,   // Right
       20, 21, 22, 22, 23, 20    // Left
    };


public:
    TestCube() {}
    ~TestCube() override {
        SDL_Log("Destroyed cube!");
    }

    void OnAwake() override
    {
        auto mesh = new Mesh(vertices, indices, true, true);

        auto* am = m_scene->GetAssetManager();
        auto shader = am->GetShader("test");
        if(!shader->IsLinked())
        {
            shader->CreateFromBinaryFile(Shader::Type::Vertex, "../../Engine/shaders/test.vert.spv");
            shader->CreateFromBinaryFile(Shader::Type::Fragment, "../../Engine/shaders/test.frag.spv");
            shader->Link();
        }

        auto texture = am->GetTexture("test");
        texture->LoadFromFile("../../../Engine/assets/wall.jpg");

        auto material = new Material();
        material->SetShader(shader);
        material->AddTexture(texture);

        meshComp = (MeshComponent*)this->AddComponent<MeshComponent>("MeshCompCube");
        meshComp->SetMesh(mesh);
        meshComp->SetMaterial(material);
    }

    int frame = 0;
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

        if(key_states[SDL_SCANCODE_V])
        {
            auto list = this->GetComponentsByType<TestComponent>();
            if(!list.empty())
                return;

            testComp = this->AddComponent<TestComponent>("TestComp");
            SDL_Log("[Cube] created i think");
        }

        if(key_states[SDL_SCANCODE_B])
        {
            if(this->GetComponentsByType<TestComponent>().empty())
                return;

            this->DestroyComponent(testComp);
        }

        if(key_states[SDL_SCANCODE_H])
        {
            for(auto handle : m_scene->FindGameObjectsByType<TestCube>())
                SDL_Log("%s", handle.gameObject->GetName().c_str());
        }

        if(key_states[SDL_SCANCODE_SPACE])
        {
            this->GetTransform()->RotateTowards(glm::vec3(0));
        }
    }

    void ProcessEvent(SDL_Event event)
    {
        if(event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_ESCAPE)
        {
            m_scene->GetAppControl()->Stop();
        }

        if(event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_8)
        {
            m_scene->GetSceneManager()->LoadScene("2");
        }
        if(event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_7)
        {
            m_scene->GetSceneManager()->LoadScene("1");
        }
    }

    void OnDestroy() override
    {
        SDL_Log("[CUBE] byebye");
    }

    MeshComponent* meshComp; // not here
    TestComponent* testComp;
};