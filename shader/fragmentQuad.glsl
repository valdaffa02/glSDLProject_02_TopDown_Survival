#version 460 core
out vec4 color;

uniform vec4 uColor;

void main()
{
	color = vec4(uColor);
}