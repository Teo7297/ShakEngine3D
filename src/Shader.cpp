#include "Shader.h"

using namespace Shak;

Shader::Shader()
{
}

Shader::~Shader()
{
    if(m_program)
        GL_CHECK(glDeleteProgram(m_program));
}

std::vector<char> Shader::ReadBinaryFile(const fs::path& filename) const
{
    std::vector<char> buffer;
    std::ifstream is(filename, std::ifstream::binary);

    if(!is)
    {
        SDL_LogError(0, "Failed to read binary file at path: %s", filename);
        return buffer;
    }

    is.seekg(0, is.end);
    int length = is.tellg();
    is.seekg(0, is.beg);

    buffer.reserve(length);
    buffer.resize(length);

    is.read(buffer.data(), length);
    is.close();

    return buffer;
}

std::string Shader::ReadTextFile(const fs::path& filename) const
{
    size_t size = 0;
    char* buf = static_cast<char*>(SDL_LoadFile(filename.string().c_str(), &size));
    if(!buf)
    {
        SDL_LogError(0, "[Shader] Could not load text file: %s", filename);
        return "";
    }

    // Construct string from pointer+size so embedded NULs are preserved.
    std::string result(buf, size);
    SDL_free(buf);
    return result;
}

bool Shader::Bind()
{
    if(!m_program)
        return false;
    GL_CHECK(glUseProgram(m_program));
    return true;
}

bool Shader::CreateFromBinaryFile(Type type, const fs::path& filename)
{
    if(!GLEW_ARB_gl_spirv)
    {
        SDL_LogError(0, "[Shader] Spir-v (binary) shaders not supported!");
        return false;
    }

    auto shader = CreateShaderHandle(type);
    auto shaderData = ReadBinaryFile(filename);
    int compiled = 0;

    GL_CHECK(glShaderBinary(1, &shader, GL_SHADER_BINARY_FORMAT_SPIR_V_ARB, shaderData.data(), shaderData.size()));
    GL_CHECK(glSpecializeShader(shader, "main", 0, NULL, NULL));
    GL_CHECK(glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled));

    if(!compiled)
    {
        SDL_LogError(0, "[Shader] Failed to load shader at: %s", filename);
        return false;
    }

    return true;
}

bool Shader::CreateFromTextFile(Type type, const fs::path& filename)
{
    auto shader = CreateShaderHandle(type);
    const char* shaderCode = ReadTextFile(filename).c_str();
    int compiled = 0;

    GL_CHECK(glShaderSource(shader, 1, &shaderCode, NULL));
    GL_CHECK(glCompileShader(shader));
    GL_CHECK(glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled));

    if(!compiled)
    {
        SDL_LogError(0, "[Shader] Failed to load shader at: %s", filename);
        return false;
    }

    return true;
}

bool Shader::CreateFromString(Type type, const std::string& code)
{
    auto shader = CreateShaderHandle(type);
    const char* shaderCode = code.c_str();
    int compiled = 0;

    GL_CHECK(glShaderSource(shader, 1, &shaderCode, NULL));
    GL_CHECK(glCompileShader(shader));
    GL_CHECK(glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled));

    if(!compiled)
    {
        SDL_LogError(0, "[Shader] Failed to compile shader:\n %s", code);
        return false;
    }

    return true;
}

bool Shader::Link()
{
    if(m_program)
    {
        SDL_LogWarn(0, "[Shader] Shader has already been linked!");
        return false;
    }

    if(m_shaderData.vertex == 0)
    {
        SDL_LogError(0, "[Shader] Vertex shader not set!");
        return false;
    }

    if(m_shaderData.fragment == 0)
    {
        SDL_LogError(0, "[Shader] Fragment shader not set!");
        return false;
    }

    m_program = glCreateProgram();

    GL_CHECK(glAttachShader(m_program, m_shaderData.vertex));
    GL_CHECK(glAttachShader(m_program, m_shaderData.fragment));
    GL_CHECK(glLinkProgram(m_program));
    GL_CHECK(glDeleteShader(m_shaderData.vertex));
    GL_CHECK(glDeleteShader(m_shaderData.fragment));

    return true;
}

GLuint Shader::CreateShaderHandle(Type type)
{
    switch(type)
    {
    case Type::Vertex:
        m_shaderData.vertex = glCreateShader(GL_VERTEX_SHADER);
        return m_shaderData.vertex;
    case Type::Fragment:
        m_shaderData.fragment = glCreateShader(GL_FRAGMENT_SHADER);
        return m_shaderData.fragment;
    }
    return 0;
}

