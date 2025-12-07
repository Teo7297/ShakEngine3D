#pragma once

#include "Includes.h"

namespace Shak
{
    class Material
    {
    public:
        Material();
        ~Material();
        void SetTexture(const std::shared_ptr<class Texture>& texture);
        void SetShader(const std::shared_ptr<class Shader>& shader);

        std::shared_ptr<class Texture> GetTexture() const;
        std::shared_ptr<class Shader> GetShader() const;

        void Bind();

    private:
        std::shared_ptr<class Texture> m_texture;
        std::shared_ptr<class Shader> m_shader;
    };
}