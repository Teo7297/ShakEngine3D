#pragma once

#include "Includes.h"
#include "GameObject.h"
#include "Mesh.h"
#include "components/MeshComponent.h"
#include "Material.h"
#include "Shader.h"
#include "CubeMap.h"
#include "components/TestComponent.h"

using namespace Shak;

class TestSkybox : public GameObject
{
    /* Cube vertices with colors (position XYZ, color RGB) */
    std::vector<float> vertices = {
        // Front face
        // x, y, z,
        -1.0f, -1.0f,  1.0f,  // Bottom-left
         1.0f, -1.0f,  1.0f,  // Bottom-right
         1.0f,  1.0f,  1.0f,  // Top-right
        -1.0f,  1.0f,  1.0f,  // Top-left

        // Back face
        -1.0f, -1.0f, -1.0f, // Bottom-right (relative to front)
        -1.0f,  1.0f, -1.0f, // Top-right
         1.0f,  1.0f, -1.0f, // Top-left
         1.0f, -1.0f, -1.0f, // Bottom-left

         // Top face
         -1.0f,  1.0f, -1.0f, // Top-left
         -1.0f,  1.0f,  1.0f, // Bottom-left
          1.0f,  1.0f,  1.0f, // Bottom-right
          1.0f,  1.0f, -1.0f, // Top-right

          // Bottom face
          -1.0f, -1.0f, -1.0f, // Bottom-left
           1.0f, -1.0f, -1.0f, // Bottom-right
           1.0f, -1.0f,  1.0f, // Top-right
          -1.0f, -1.0f,  1.0f, // Top-left

          // Right face
           1.0f, -1.0f, -1.0f, // Bottom-right
           1.0f,  1.0f, -1.0f, // Top-right
           1.0f,  1.0f,  1.0f, // Top-left
           1.0f, -1.0f,  1.0f, // Bottom-left

           // Left face
           -1.0f, -1.0f, -1.0f, // Bottom-left
           -1.0f, -1.0f,  1.0f, // Bottom-right
           -1.0f,  1.0f,  1.0f, // Top-right
           -1.0f,  1.0f, -1.0f // Top-left
    };

    std::vector<unsigned int> indices = {
        0,  1,  2,  2,  3,  0,   // Front
        4,  5,  6,  6,  7,  4,   // Back
        8,  9, 10, 10, 11,  8,   // Top
       12, 13, 14, 14, 15, 12,   // Bottom
       16, 17, 18, 18, 19, 16,   // Right
       20, 21, 22, 22, 23, 20    // Left
    };


    void OnAwake() override
    {
        auto mesh = std::make_shared<Mesh>(vertices, indices, false, false);

        auto shader = std::make_shared<Shader>();
        shader->CreateFromBinaryFile(Shader::Type::Vertex, "../shaders/cubemap.vert.spv");
        shader->CreateFromBinaryFile(Shader::Type::Fragment, "../shaders/cubemap.frag.spv");
        shader->Link();

        auto texture = std::make_shared<CubeMap>();
        texture->LoadFromFile("../../assets/cubemap.png");

        auto material = std::make_shared<Material>();
        material->SetShader(shader);
        material->AddTexture(texture);

        meshComp = (MeshComponent*)this->AddComponent<MeshComponent>("MeshCompSky");
        meshComp->SetMesh(std::move(mesh));
        meshComp->SetMaterial(std::move(material));
        meshComp->SetIsSkybox(true);
    }

    void OnDestroy() override
    {
    }

public:
    MeshComponent* meshComp;
};
