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

int Material::AddTexture(const std::shared_ptr<Texture>& texture)
{
    m_textures.emplace_back(texture);
    return m_textures.size() - 1;
}

void Material::SetShader(const std::shared_ptr<Shader>& shader)
{
    m_shader = shader;
}

std::shared_ptr<Texture> Shak::Material::GetTexture(int index) const
{
    if(index >= m_textures.size())
    {
        SDL_LogError(0, "[Material] Tried to get texture #%i but there are only %i available", index, m_textures.size());
        return nullptr;
    }
    return m_textures[index];
}

std::shared_ptr<Shader> Shak::Material::GetShader() const
{
    return m_shader;
}

void Material::BindTextures()
{
    for(int slot = 0; slot < m_textures.size(); slot++)
    {
        GL_CHECK(glActiveTexture(GL_TEXTURE0 + slot));
        m_textures[slot]->Bind();
    }
}
