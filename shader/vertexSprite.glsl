#version 460 core
layout (location = 0) in vec4 vertex;		// <vec2 position, vec2 texCoords>

out vec2 TexCoords;

uniform mat4 uModel;
uniform mat4 uProjection;
uniform vec4 uTextureOffset;
uniform int uHorizontalFlip;

void main()
{
	vec2 newTexCoords = vertex.zw;

	if(uTextureOffset.z < 1.0 && uTextureOffset.w < 1.0)
	{
		if(uHorizontalFlip == 0)
		{
			if((vertex.z == 0) && (vertex.w == 0))
			{
				newTexCoords.x = (uTextureOffset.x * uTextureOffset.z);
				newTexCoords.y = (uTextureOffset.y * uTextureOffset.w);
			}
			else if((vertex.z == 1) && (vertex.w == 0))
			{
				newTexCoords.x = (uTextureOffset.x * uTextureOffset.z) + uTextureOffset.z;
				newTexCoords.y = (uTextureOffset.y * uTextureOffset.w);
			}
			else if((vertex.z == 1) && (vertex.w == 1))
			{
				newTexCoords.x = (uTextureOffset.x * uTextureOffset.z) + uTextureOffset.z;
				newTexCoords.y = (uTextureOffset.y * uTextureOffset.w) + uTextureOffset.w;
			}
			else if((vertex.z == 0) && (vertex.w == 1))
			{
				newTexCoords.x = (uTextureOffset.x * uTextureOffset.z);
				newTexCoords.y = (uTextureOffset.y * uTextureOffset.w) + uTextureOffset.w;
			}
		}
		else
		{
			if((vertex.z == 0) && (vertex.w == 0))
			{
				newTexCoords.x = (uTextureOffset.x * uTextureOffset.z) + uTextureOffset.z;
				newTexCoords.y = (uTextureOffset.y * uTextureOffset.w);
				
			}
			else if((vertex.z == 1) && (vertex.w == 0))
			{
				newTexCoords.x = (uTextureOffset.x * uTextureOffset.z);
				newTexCoords.y = (uTextureOffset.y * uTextureOffset.w);
			}
			else if((vertex.z == 1) && (vertex.w == 1))
			{
				newTexCoords.x = (uTextureOffset.x * uTextureOffset.z);
				newTexCoords.y = (uTextureOffset.y * uTextureOffset.w) + uTextureOffset.w;
			}
			else if((vertex.z == 0) && (vertex.w == 1))
			{
				newTexCoords.x = (uTextureOffset.x * uTextureOffset.z) + uTextureOffset.z;
				newTexCoords.y = (uTextureOffset.y * uTextureOffset.w) + uTextureOffset.w;
			}
		}
	}
	
	TexCoords = newTexCoords;
	gl_Position = uProjection * uModel * vec4(vertex.xy, 0.0, 1.0);
}