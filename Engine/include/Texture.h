#pragma once

#include "Includes.h"

namespace Shak
{
    class Texture
    {
    public:
        Texture();
        virtual ~Texture();
        virtual bool LoadFromFile(const fs::path& filename);
        void Bind();

    protected:
        GLuint m_textureID;
        GLint m_glTextureType;
    };
}