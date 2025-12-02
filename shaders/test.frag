#version 460 core
layout(location = 0) out vec4 FragColor;

layout(location = 0) in vec3 vertexColor;

void main()
{
    FragColor = vec4(vertexColor, 1.0);
}