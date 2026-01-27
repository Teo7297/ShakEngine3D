#pragma once

#include <Material.h>
#include <Shader.h>

using namespace Shak;

class TestMaterial : public Material
{
public:
    void SetInstanceSpecificUniforms() override
    {
        std::string uniformName = "uColor";
        m_shader->SetUniformColor(1, Color);
        // m_shader->SetUniformColor(m_shader->GetUniformLocation(uniformName), Color);
    }

    glm::vec4 Color = glm::vec4(1.f);
};