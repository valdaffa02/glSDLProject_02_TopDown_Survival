#pragma once

#include <string>
#include "SpriteSheet.h"
#include "SpriteRenderer.h"


class Map
{
private:
	SpriteRenderer* renderer;
	SpriteSheet* spriteSheet;
	float mapScale;
	float tileSize;
	float scaledSize;

public:
	Map(SpriteRenderer* renderer, SpriteSheet* spriteSheet, const float& mapScale, const float& tileSize);

	~Map();

	void LoadMap(const std::string& path, const int& sizeX, const int& sizeY);
	void AddTile(const int& xIndex, const int& yIndex, const float& xPosition, const float& yPosition);
};