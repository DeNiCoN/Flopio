#version 450 core
#extension GL_ARB_bindless_texture : require

out vec4 FragColor;
in vec2 textCoord;
in flat uvec2 textureH;
void main()
{
	sampler2D text = sampler2D(textureH);
    vec4 texColor = texture(text, textCoord);
	if(texColor.a == 0.0)
        discard;
	FragColor = texColor;
}