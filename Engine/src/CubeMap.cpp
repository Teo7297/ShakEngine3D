#include "CubeMap.h"

#include <stb_image.h>

using namespace Shak;

CubeMap::CubeMap()
{
    m_glTextureType = GL_TEXTURE_CUBE_MAP;
}

CubeMap::~CubeMap()
{
}

bool CubeMap::LoadFromFile(const fs::path& filename)
{
    stbi_set_flip_vertically_on_load(true);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(filename.string().c_str(), &width, &height, &nrChannels, 0);

    if(!data)
    {
        SDL_LogError(0, "[CubeMap] Could not load texture from %s", filename.string().c_str());
        return false;
    }

    GL_CHECK(glGenTextures(1, &m_textureID));
    GL_CHECK(glBindTexture(m_glTextureType, m_textureID));

    GL_CHECK(glTexParameteri(m_glTextureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GL_CHECK(glTexParameteri(m_glTextureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GL_CHECK(glTexParameteri(m_glTextureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL_CHECK(glTexParameteri(m_glTextureType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GL_CHECK(glTexParameteri(m_glTextureType, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

    int widthStep = width / 4;
    int heightStep = height / 3;

    auto textureCoords = ExtractTextureCoords(widthStep, heightStep);

    for(int i = 0; i < 6; i++)
    {
        std::vector<unsigned char> faceData(widthStep * heightStep * nrChannels);
        for(int y = 0; y < heightStep; y++)
        {
            int sourceRow = textureCoords[i].start_y + y;
            int sourceIndex = (width * sourceRow + textureCoords[i].start_x) * nrChannels;

            int destIndex = y * widthStep * nrChannels;

            int rowSize = widthStep * nrChannels;

            memcpy(&faceData[destIndex], &data[sourceIndex], rowSize);
        }

        auto format = nrChannels == 3 ? GL_RGB : GL_RGBA;

        GL_CHECK(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, widthStep, heightStep, 0, format, GL_UNSIGNED_BYTE, faceData.data()));
    }

    stbi_image_free(data);

    return true;
}

std::array<CubeMap::CubeMapTextureCoords, 6> CubeMap::ExtractTextureCoords(int widthStep, int heightStep)
{
    std::array<CubeMapTextureCoords, 6> result;

    result[0] = { .start_x = widthStep * 2, .start_y = heightStep * 1 }; // RIGHT
    result[1] = { .start_x = widthStep * 0, .start_y = heightStep * 1 }; // LEFT
    result[2] = { .start_x = widthStep * 1, .start_y = heightStep * 0 }; // UP
    result[3] = { .start_x = widthStep * 1, .start_y = heightStep * 2 }; // DOWN
    result[4] = { .start_x = widthStep * 1, .start_y = heightStep * 1 }; // FRONT
    result[5] = { .start_x = widthStep * 3, .start_y = heightStep * 1 }; // BACK

    return result;
}
