#version 450 core

out vec4 FragColor;
in vec2 textCoord;

layout(binding=0) uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, textCoord);
}