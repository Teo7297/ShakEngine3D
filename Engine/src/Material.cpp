#include "Material.h"
#include "Texture.h"
#include "Shader.h"

using namespace Shak;

Material::Material()
    : m_textures{}, m_shader{ nullptr }
{
}

Material::~Material()
{

}

int Material::AddTexture(Texture* texture)
{
    m_textures.emplace_back(texture);
    return m_textures.size() - 1;
}

void Material::SetShader(Shader* shader)
{
    m_shader = shader;
}

Texture* Material::GetTexture(int index) const
{
    if(index >= m_textures.size())
    {
        SDL_LogError(0, "[Material] Tried to get texture #%i but there are only %i available", index, m_textures.size());
        return nullptr;
    }
    return m_textures[index];
}

Shader* Material::GetShader() const
{
    return m_shader;
}

void Material::SetTextureUniforms()
{
    for(int slot = 0; slot < m_textures.size(); slot++)
    {
        std::string uniformName = "uTexture";
        if(slot > 0)
            uniformName.append(std::to_string(slot));
        m_shader->SetUniformInt(m_shader->GetUniformLocation("uTexture"), 0);
    }
}

void Material::BindTextures()
{
    for(int slot = 0; slot < m_textures.size(); slot++)
    {
        GL_CHECK(glActiveTexture(GL_TEXTURE0 + slot));
        m_textures[slot]->Bind();
    }
}
