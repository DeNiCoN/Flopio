#version 450 core
//#extension GL_ARB_bindless_texture : require

out vec4 FragColor;
in vec2 textCoord;
in flat uvec2 textureH;
void main()
{
	FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    //vec4 texColor = texture(sampler2D(textureH), textCoord);
	//if(texColor.a == 0.0)
    //    discard;
}