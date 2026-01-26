#pragma once

#include <UI/UIWIdget.h>
#include <Mesh.h>
#include <Material.h>
#include <AssetManager.h>
#include <Shader.h>

using namespace Shak;

class TestWidget : public UIWidget
{
    std::vector<float> vertices =
    {
        //  x    y    r     g     b
            0.f, 0.f, 1.0f, 0.0f, 0.0f,
            0.f, 1.f, 1.0f, 0.0f, 0.0f,
            1.f, 1.f, 1.0f, 0.0f, 0.0f,
            1.f, 0.f, 1.0f, 0.0f, 0.0f
    };

    std::vector<unsigned int> indices =
    {
        0, 1, 2, 0, 2, 3
    };

    void Initialize() override
    {
        m_mesh = new Mesh(vertices, indices, true, false);

        auto* am = new AssetManager();
        auto shader = am->GetShader("plain");
        if(!shader->IsLinked())
        {
            shader->CreateFromBinaryFile(Shader::Type::Vertex, "../../Engine/shaders/plain.vert.spv");
            shader->CreateFromBinaryFile(Shader::Type::Fragment, "../../Engine/shaders/plain.frag.spv");
            shader->Link();
        }

        m_material = new Material();
        m_material->SetShader(shader);

        m_screenCoords = { 50.f, 50.f };
        m_active = true;
    }
};