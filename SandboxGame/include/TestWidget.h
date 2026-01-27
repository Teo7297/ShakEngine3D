#pragma once

#include <UI/UIWIdget.h>
#include <Mesh.h>
#include "TestMaterial.h"
#include <AssetManager.h>
#include <Shader.h>

using namespace Shak;

class TestWidget : public UIWidget
{
    std::vector<float> vertices =
    {
        //  x    y    z    r     g     b
            0.f, 0.f, 0.f, 1.0f, 0.0f, 0.0f,
            0.f, 1.f, 0.f, 1.0f, 0.0f, 0.0f,
            1.f, 1.f, 0.f, 1.0f, 0.0f, 0.0f,
            1.f, 0.f, 0.f, 1.0f, 0.0f, 0.0f
    };

    std::vector<unsigned int> indices =
    {
        0, 1, 2, 0, 2, 3
    };

    void Red()
    {
        ((TestMaterial*)m_material)->Color = glm::vec4(1.f, 0.f, 0.f, 1.f);
    }

    void Green()
    {
        ((TestMaterial*)m_material)->Color = glm::vec4(0.f, 1.f, 0.f, 1.f);
    }

    void Blue()
    {
        ((TestMaterial*)m_material)->Color = glm::vec4(0.f, 0.f, 1.f, 1.f);
    }

public:
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

        auto* material = new TestMaterial();
        material->Color = glm::vec4(0.0, 1.0, 0.0, 1.0);
        material->SetShader(shader);
        m_material = material;

        m_screenCoords = { 50.f, 50.f };
        m_active = true;
    }

    void ProcessEvent(SDL_Event event) override
    {
        switch(event.type)
        {
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            Blue();
            break;
        case SDL_EVENT_MOUSE_BUTTON_UP:
            Red();
            break;
        case SDL_EVENT_MOUSE_MOTION:
            break;
        default:
            break;
        };
    }
};
