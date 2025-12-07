#include "Material.h"
#include "Texture.h"
#include "Shader.h"

using namespace Shak;

Material::Material()
    : m_texture{ nullptr }, m_shader{ nullptr }
{
}

Material::~Material()
{

}

void Material::SetTexture(const std::shared_ptr<class Texture>& texture)
{
    m_texture = texture;
}

void Material::SetShader(const std::shared_ptr<class Shader>& shader)
{
    m_shader = shader;
}

std::shared_ptr<Texture> Shak::Material::GetTexture() const
{
    return m_texture;
}

std::shared_ptr<Shader> Shak::Material::GetShader() const
{
    return m_shader;
}

void Material::Bind()
{
    if(m_texture)
        m_texture->Bind();
    if(m_shader)
        m_shader->Bind();
}
