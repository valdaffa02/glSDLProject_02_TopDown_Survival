#pragma once
#include "Components.h"
#include "../SpriteSheet.h"
#include "glm/glm.hpp"

#include <iostream>


class TileComponent : public Component
{
public:
	SpriteSheet* spriteSheet = nullptr;
	SpriteRenderer* renderer = nullptr;
	glm::vec2 position;
	glm::vec2 size;
	glm::ivec2 textureIndex;

	glm::vec4 color;

	TileComponent() = default;

	~TileComponent()
	{}

	TileComponent(SpriteRenderer* spriteRenderer, SpriteSheet* spriteSheet, const int& xIndex, const int& yIndex, const float& xPosition, const float& yPosition, const float& tileSize, const float& scale = 1, glm::vec4 color = glm::vec4(1.0))
		: renderer(spriteRenderer), spriteSheet(spriteSheet), textureIndex(xIndex, yIndex), position(xPosition, yPosition), size(tileSize * scale), color(color)
	{}

	void Update(float& deltaTime) override
	{}

	void Draw() override
	{
		if(spriteSheet != nullptr && renderer != nullptr)
		{
			renderer->DrawSpriteFromSheet(this->spriteSheet, this->textureIndex, this->position, this->size, false, 0.0f, this->color);
		}
		else
		{
			std::cout << "Error: nullpointer present in TileComponent!\n";
		}
		
	}
};