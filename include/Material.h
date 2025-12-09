#pragma once

#include "Includes.h"

namespace Shak
{
    class Material
    {
    public:
        Material();
        ~Material();
        int AddTexture(const std::shared_ptr<class Texture>& texture);
        void SetShader(const std::shared_ptr<class Shader>& shader);

        std::shared_ptr<class Texture> GetTexture(int index) const;
        std::shared_ptr<class Shader> GetShader() const;

        void BindTextures();

    private:
        std::vector<std::shared_ptr<class Texture>> m_textures;
        std::shared_ptr<class Shader> m_shader;
    };
}