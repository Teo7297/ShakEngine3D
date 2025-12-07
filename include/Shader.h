#pragma once

#include <Includes.h>

namespace Shak
{
    class Shader
    {
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
        Shader();
        ~Shader();
        bool Bind();
        bool CreateFromBinaryFile(Type type, const fs::path& filename);
        bool CreateFromTextFile(Type type, const fs::path& filename);
        bool CreateFromString(Type type, const std::string& code);
        bool Link();

    private:
        std::vector<char> ReadBinaryFile(const fs::path& filename) const;
        std::string ReadTextFile(const fs::path& filename) const;
        GLuint CreateShaderHandle(Type type);

    private:
        GLuint m_program;
        ShadersData m_shaderData;
    };
}