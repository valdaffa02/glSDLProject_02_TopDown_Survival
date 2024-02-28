#version 460 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D uImage;
uniform vec4 uSpriteColor;

void main()
{
	color = vec4(uSpriteColor) * texture(uImage, TexCoords);
}