#version 460 core

layout(location = 0) in vec3 aPos;

layout(location = 0) out vec3 TexCoords;

layout(std140, binding = 0) uniform MatrixBlock
{
    mat4 model;
    mat4 viewProjection;
}
ubo;

void main()
{
    TexCoords = aPos;

    //? xyww sets the depth at max value (1.0) and allows to render the skybox as last. saving lots of fragments executions.
    gl_Position = (ubo.viewProjection * vec4(aPos, 1.0)).xyww;
}