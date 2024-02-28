#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include "Renderer.h"
#include "ResourceManager.h"
#include "Time.h"
#include "Map.h"

#include "ECS/Components.h"



class Game
{
private:

	

	SDL_Window* gWindow;
	SDL_GLContext gContext;
	

	float windowWidth, windowHeight;

	Renderer* renderer;
	Time* time;

	std::vector<Map*> maps;

	bool keyPress[255] = { false };
	

public:
	enum GameState
	{
		RUNNING,
		PAUSED,
		QUIT
	};
	
	GameState gState;

	int mouseX, mouseY;
	glm::vec2 mousePosition;
	// Constructor
	Game();

	// Deconstructor
	~Game();

	void Init(const char* windowTitle, int windowWidth, int windowHeight);

	void HandleEvents();

	void Update();

	void Draw();

	void Clean();

	void SyncMousePosition();

	void CollisionCheck();

	void CreateProjectile(glm::vec2 sourcePosition, glm::vec2 targetPosition, int range, int speed);

	void SpawnEnemy(glm::vec4 spawnArea);

	void LivingCheck();

	// WIP
	void AlignUI();

	GameState GameStatus() { return gState; }


	enum groupLabels : std::size_t
	{
		groupPlayer,
		groupEnemy,
		groupEnvironment,
		groupCollider,
		groupMap,
		groupProjectile,
		groupUI
	};

};