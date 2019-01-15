#version 450 core

out vec4 FragColor;
in vec2 textCoord;

layout(binding=0) uniform sampler2D ourTexture;

void main()
{
    vec4 texColor = texture(ourTexture, textCoord);
	if(texColor.a == 0.0)
        discard;
	FragColor = texColor;
}