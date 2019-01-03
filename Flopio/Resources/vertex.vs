#version 450 core

layout (location = 0) in vec2 aPos;

out vec2 textCoord;

uniform mat4 model;
uniform mat4 projectionView;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, 1.0, 1.0);
	textCoord = aPos + 0.5;
}