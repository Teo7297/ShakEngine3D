#pragma once

#include "Includes.h"

namespace Shak
{
    class Texture;
    class Shader;
    class Material
    {
    public:
        Material();
        virtual ~Material();
        int AddTexture(Texture* texture);
        void SetShader(Shader* shader);

        Texture* GetTexture(int index) const;
        Shader* GetShader() const;

        /// @brief Uploads the texture uniforms to the shader.
        /// Texture names are uTexture for texture 0, then uTextureN where N is the # of the texture slot (1,2,3,etc)
        void BindTextures();

        virtual void SetInstanceSpecificUniforms() {}

    protected:
        std::vector<Texture*> m_textures;
        Shader* m_shader;
    };
}