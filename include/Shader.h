#pragma once

#include <Includes.h>

namespace Shak
{
    struct MatrixBlock
    {
        glm::mat4 model;
        glm::mat4 viewProjection;
    };

    class Shader
    {
        friend class AssetManager;
    private:
        struct ShadersData
        {
            GLuint vertex = 0;
            GLuint fragment = 0;
        };

    public:
        enum class Type
        {
            Vertex,
            Fragment
        };

    public:
        ~Shader();
        bool Bind();
        bool CreateFromBinaryFile(Type type, const fs::path& filename);
        bool CreateFromTextFile(Type type, const fs::path& filename);
        bool CreateFromString(Type type, const std::string& code);
        bool Link();

        GLint GetUniformLocation(const std::string& name);
        void SetMVP(MatrixBlock matrices); // TODO: remove data copy
        void SetUniformFloat(GLuint loc, float value);
        void SetUniformInt(GLuint loc, int value);

    private:
        Shader();
        std::vector<char> ReadBinaryFile(const fs::path& filename) const;
        std::string ReadTextFile(const fs::path& filename) const;
        GLuint CreateShaderHandle(Type type);

    private:
        GLuint m_program, m_ubo;
        ShadersData m_shaderData;
    };
}