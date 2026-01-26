#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
// layout(location = 2) in vec2 aTexCoord;

layout(std140, binding = 0) uniform MatrixBlock
{
    mat4 model;
    mat4 viewProjection;
}
ubo;

layout(location = 0) out vec3 fragColor;
// layout(location = 1) out vec2 texCoord;

void main()
{
    gl_Position = ubo.viewProjection * ubo.model * vec4(aPos, 1.0);
    fragColor = aColor;
    // texCoord = aTexCoord;
}
