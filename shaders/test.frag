#version 460 core

// In
layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 texCoord;

// Uniform
layout(location = 0) uniform float uTime = 1.0;
layout(location = 1) uniform sampler2D uTexture;

// Out
layout(location = 0) out vec4 FragColor;

void main()
{
    FragColor = vec4(fragColor, 1.0) * texture(uTexture, texCoord);
}