#include "Texture.h"

#include <stb_image.h>

using namespace Shak;

Texture::Texture()
    : m_textureID{ 0 }
    , m_glTextureType{ GL_TEXTURE_2D }
{

}

Texture::~Texture()
{
    glDeleteTextures(1, &m_textureID);
}

bool Texture::LoadFromFile(const fs::path& filename)
{
    stbi_set_flip_vertically_on_load(true);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(filename.string().c_str(), &width, &height, &nrChannels, 0);

    if(!data)
    {
        SDL_LogError(0, "[Texture] Could not load texture from %s", filename.string().c_str());
        return false;
    }

    auto format = nrChannels == 3 ? GL_RGB : GL_RGBA;

    GL_CHECK(glGenTextures(1, &m_textureID));
    GL_CHECK(glBindTexture(m_glTextureType, m_textureID));

    GL_CHECK(glTexParameteri(m_glTextureType, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GL_CHECK(glTexParameteri(m_glTextureType, GL_TEXTURE_WRAP_T, GL_REPEAT));
    GL_CHECK(glTexParameteri(m_glTextureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    GL_CHECK(glTexParameteri(m_glTextureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    GL_CHECK(glTexImage2D(m_glTextureType, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data));
    GL_CHECK(glGenerateMipmap(m_glTextureType));

    stbi_image_free(data);

    return true;
}

void Texture::Bind()
{
    GL_CHECK(glBindTexture(m_glTextureType, m_textureID));
}
