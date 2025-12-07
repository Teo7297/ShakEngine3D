#pragma once

#include "Includes.h"

namespace Shak
{
    class Texture
    {
    public:
        Texture();
        ~Texture();
        bool LoadFromFile(const fs::path& filename);
        void Bind();

    private:
        GLuint m_textureID;
    };
}