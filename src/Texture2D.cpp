#include "Texture2D.h"


// Constructor
Texture2D::Texture2D() : width(0), height(0), internalFormat(GL_RGB), imageFormat(GL_RGB), wrapS(GL_REPEAT), wrapT(GL_REPEAT), filterMin(GL_NEAREST), filterMax(GL_NEAREST)
{
	glGenTextures(1, &this->ID);
}

// Generate texture from image data
void Texture2D::Generate(SDL_Surface* data)
{
	this->width = data->w;
	this->height = data->h;
	
	// Create texture
	glBindTexture(GL_TEXTURE_2D, this->ID);
	glTexImage2D(GL_TEXTURE_2D, 0, this->internalFormat, data->w, data->h, 0, this->imageFormat, GL_UNSIGNED_BYTE, data->pixels);

	// Set texture wrap and filter modes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->filterMin);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->filterMax);

	// Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

// Binds the texture as the current active GL_TEXTURE_2D texture object
void Texture2D::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, this->ID);
}