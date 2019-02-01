#version 450 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in uvec2 texture;
layout (location = 2) in mat4 model;

out vec2 textCoord;
out uvec2 textureH;

uniform mat4 projectionView;

void main()
{
    gl_Position = projectionView * model * vec4(aPos.x, aPos.y, 1.0, 1.0);
	textCoord = aPos + 0.5;
	textureH = texture;
}