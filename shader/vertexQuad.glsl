#version 460 core
in vec2 vertex;		// <vec2 position>

uniform mat4 uModel;
uniform mat4 uProjection;

void main()
{	
	gl_Position = uProjection * uModel * vec4(vertex.xy, 0.0, 1.0);
}