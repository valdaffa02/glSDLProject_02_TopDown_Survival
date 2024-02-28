#pragma once

#include <glew.h>
#include <gl/glu.h>

#include <SDL.h>

class Texture2D
{
public:
	GLuint ID;

	GLuint width;
	GLuint height;

	GLuint internalFormat;
	GLuint imageFormat;

	GLuint wrapS;
	GLuint wrapT;
	GLuint filterMin;			// filtering mode if texture pixels < screen pixels
	GLuint filterMax;			// filtering mode if texture pixels > screen pixels

	// Constructor
	Texture2D();

	// Generate texture from image data
	void Generate(SDL_Surface* data);

	// Binds the texture as the current active GL_TEXTURE_2D texture object
	void Bind() const;
};