#version 460 core

layout(location = 0) in vec3 TexCoords;

layout(location = 0) out vec4 FragColor;

layout(location = 0) uniform samplerCube uTexture;

void main()
{
    FragColor = texture(uTexture, TexCoords);
}