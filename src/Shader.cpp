#include "Shader.h"

using namespace Shak;

//TODO: use Direct State Access commands instead of legacy ones. they avoid having to bind the shaders!

Shader::Shader()
    : m_program(0)
{
    // Create the buffer
    glCreateBuffers(1, &m_ubo);

    // Allocate memory for the buffer (sizeof(MatrixBlock))
    // GL_DYNAMIC_STORAGE_BIT allows us to update it later with glNamedBufferSubData
    glNamedBufferStorage(m_ubo, sizeof(MatrixBlock), nullptr, GL_DYNAMIC_STORAGE_BIT);

    // BINDING POINT: This links the buffer to 'binding = 0' in the shader
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_ubo);
}

Shader::~Shader()
{
    // In the case this gets destroyed without ever being linked
    if(m_shaderData.fragment)
        GL_CHECK(glDeleteShader(m_shaderData.fragment));
    if(m_shaderData.vertex)
        GL_CHECK(glDeleteShader(m_shaderData.vertex));

    if(m_program)
        GL_CHECK(glDeleteProgram(m_program));

    if(m_ubo)
        GL_CHECK(glDeleteBuffers(1, &m_ubo));
}

std::vector<char> Shader::ReadBinaryFile(const fs::path& filename) const
{
    std::vector<char> buffer;
    std::ifstream is(filename, std::ifstream::binary);

    if(!is)
    {
        SDL_LogError(0, "Failed to read binary file at path: %s", filename.string().c_str());
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

    if(m_ubo)
        GL_CHECK(glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_ubo));

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

    GL_CHECK(m_program = glCreateProgram());
    GL_CHECK(glAttachShader(m_program, m_shaderData.vertex));
    GL_CHECK(glAttachShader(m_program, m_shaderData.fragment));
    GL_CHECK(glLinkProgram(m_program));
    GL_CHECK(glDeleteShader(m_shaderData.vertex));
    m_shaderData.vertex = 0;
    GL_CHECK(glDeleteShader(m_shaderData.fragment));
    m_shaderData.fragment = 0;

    return true;
}

GLint Shader::GetUniformLocation(const std::string& name)
{
    GLint loc;
    GL_CHECK(loc = glGetUniformLocation(m_program, name.c_str()));
    return loc;
}

void Shader::SetMVP(MatrixBlock matrices)
{
    GL_CHECK(glNamedBufferSubData(m_ubo, 0, sizeof(MatrixBlock), &matrices));
}

void Shader::SetUniformFloat(GLuint loc, float value)
{
    if(!m_program) return;
    GL_CHECK(glProgramUniform1f(m_program, loc, value));
}

void Shader::SetUniformInt(GLuint loc, int value)
{
    if(!m_program) return;
    GL_CHECK(glProgramUniform1i(m_program, loc, value));
}

GLuint Shader::CreateShaderHandle(Type type)
{
    switch(type)
    {
    case Type::Vertex:
        GL_CHECK(m_shaderData.vertex = glCreateShader(GL_VERTEX_SHADER));
        return m_shaderData.vertex;
    case Type::Fragment:
        GL_CHECK(m_shaderData.fragment = glCreateShader(GL_FRAGMENT_SHADER));
        return m_shaderData.fragment;
    }
    return 0;
}

