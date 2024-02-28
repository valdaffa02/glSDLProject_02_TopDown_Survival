#include "Map.h"
#include "ECS/Components.h"
#include "Game.h"
#include <fstream>

extern Manager manager;

Map::Map(SpriteRenderer* renderer, SpriteSheet* spriteSheet, const float& mapScale, const float& tileSize) : renderer(renderer), spriteSheet(spriteSheet), mapScale(mapScale), tileSize(tileSize)
{
	scaledSize = mapScale * tileSize;
}

Map::~Map()
{}

void Map::LoadMap(const std::string& path, const int& sizeX, const int& sizeY)
{
	char c;
	std::fstream mapFile;
	mapFile.open(path);

	int srcX, srcY;

	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			mapFile.get(c);
			srcY = atoi(&c);
			mapFile.get(c);
			srcX = atoi(&c);
			AddTile(srcX, srcY, x * scaledSize, y * scaledSize);
			//std::cout << srcX << ", " << srcY << "\n";
			//to ignore the comas
			mapFile.ignore();
		}
	}

	//to ignore the empty line
	mapFile.ignore();

	//to give collision to the map
	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			mapFile.get(c);
			if (c == '1')
			{
				auto& tcol(manager.AddEntity());
				tcol.AddComponent<ColliderComponent>(this->renderer, glm::vec2(x * scaledSize, y * scaledSize), glm::vec2(scaledSize, scaledSize));
				tcol.AddGroup(Game::groupCollider);
			}
			//to ignore the comas
			mapFile.ignore();
		}
	}

	mapFile.close();
}

void Map::AddTile(const int& xIndex, const int& yIndex, const float& xPosition, const float& yPosition)
{
	// adding tile to manager as entity
	auto& tile(manager.AddEntity());
	tile.AddComponent<TileComponent>(this->renderer, this->spriteSheet, xIndex, yIndex, xPosition, yPosition, tileSize, mapScale);
	tile.AddGroup(Game::groupMap);
}