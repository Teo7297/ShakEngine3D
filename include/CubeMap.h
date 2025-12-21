#pragma once

#include "Includes.h"
#include "Texture.h"

namespace Shak
{
    class CubeMap : public Texture
    {
    private:
        struct CubeMapTextureCoords
        {
            int start_x, start_y;
        };

    public:
        CubeMap();
        ~CubeMap();
        bool LoadFromFile(const fs::path& filename) override;

    private:
        std::array<CubeMapTextureCoords, 6> ExtractTextureCoords(int widthStep, int heightStep);

    private:
    };
}