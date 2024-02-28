#pragma once

#include "Texture2D.h"

class SpriteSheet : public Texture2D {
public:
	// dimension for individual textures in spriteSheet
	GLuint textureWidth;
	GLuint textureHeight;

	SpriteSheet();

	SpriteSheet(GLuint textureWidth, GLuint textureHeight);


};