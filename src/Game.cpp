#include "Game.h"

#include "Collision.h"
#include "Math.h"

#include <iostream>

#include <glm/glm.hpp>


Manager manager;

// Constructor
Game::Game(){}

// Deconstructor
Game::~Game(){}

auto& player(manager.AddEntity());
auto& playerHealthBar(manager.AddEntity());

void Game::Init(const char* windowTitle, int windowWidth, int windowHeight)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Error: SDL Initialization failed!" << std::endl;
		exit(1);
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	gWindow = SDL_CreateWindow("glSDL Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_OPENGL);
	if (gWindow == nullptr)
	{
		std::cout << "Error: SDL Window Initialization failed!" << std::endl;
	}

	gContext = SDL_GL_CreateContext(gWindow);
	if (gContext == nullptr)
	{
		std::cout << "Error: SDL_GL_Context Createn failed!" << std::endl;
	}

	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;

	renderer = new Renderer(windowWidth, windowHeight);

	renderer->Init();

	gState = RUNNING;

	time = new Time();

	time->TimeInit(60);			// set frameRate (FPS) to 60

	time->TimeUpdate();
	time->TimeUpdateLate();


	ResourceManager::LoadTexture("assets/awesomeface.png", true, "player");
	ResourceManager::LoadTexture("assets/ColTex.png", true, "enemy");
	ResourceManager::LoadTexture("assets/red_ball.png", true, "ball");
	ResourceManager::LoadTexture("assets/background.png", false, "background");
	ResourceManager::LoadTexture("assets/collision_box.png", true, "collisionBox");
	ResourceManager::LoadSpriteSheet("assets/test_player_anim.png", true, "playerSpriteSheet", 16, 32);
	ResourceManager::LoadSpriteSheet("assets/serene_terrain_ss.png", true, "mapSpriteSheet", 32, 32);

	Map* map = new Map(renderer->GetSpriteRenderer(), &ResourceManager::GetSpriteSheet("mapSpriteSheet"), 2, 32);
	maps.push_back(map);
	maps[0]->LoadMap("assets/map.map", 25, 20);


	player.AddComponent<TransformComponent>(glm::vec2((windowWidth / 2) - 16, (windowHeight / 2) - 32), glm::vec2(32.0f, 64.0f), glm::vec2(0.0f), 1, 100);
	player.AddComponent<StatComponent>(100, 100, 25);
	player.AddComponent<SpriteComponent>(renderer->GetSpriteRenderer(), &ResourceManager::GetSpriteSheet("playerSpriteSheet"), glm::vec4(1.0f), 0.0f, true);
	player.GetComponent<SpriteComponent>().AddAnimation("Idle1", 0, 6, 100);
	player.GetComponent<SpriteComponent>().AddAnimation("Idle2", 1, 6, 100);
	player.GetComponent<SpriteComponent>().AddAnimation("Idle3", 2, 6, 100);
	player.GetComponent<SpriteComponent>().AddAnimation("Walk1", 3, 6, 100);
	player.GetComponent<SpriteComponent>().AddAnimation("Walk2", 4, 6, 100);
	player.GetComponent<SpriteComponent>().AddAnimation("Walk3", 5, 6, 100);
	player.GetComponent<SpriteComponent>().PlayAnimation("Idle1");
	player.AddComponent<ColliderComponent>(renderer->GetSpriteRenderer(), false);

	playerHealthBar.AddComponent<TransformComponent>(glm::vec2(10, 10), glm::vec2(256.0f, 32.0f), glm::vec2(0.0f), 1, 0);
	playerHealthBar.AddComponent<SpriteComponent>(renderer->GetSpriteRenderer(), &ResourceManager::GetTexture("collisionBox"), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

	renderer->SetCameraFocus(&player.GetComponent<TransformComponent>().center);
	//std::cout << player.GetComponent<TransformComponent>().center.x << " ,  " << player.GetComponent<TransformComponent>().center.y << "\n";

	
	player.AddGroup(Game::groupPlayer);
	playerHealthBar.AddGroup(Game::groupUI);
}

auto& players(manager.GetGroup(Game::groupPlayer));
auto& enemies(manager.GetGroup(Game::groupEnemy));
auto& tiles(manager.GetGroup(Game::groupMap));
auto& mapColliders(manager.GetGroup(Game::groupCollider));
auto& projectiles(manager.GetGroup(Game::groupProjectile));
auto& uis(manager.GetGroup(Game::groupUI));


void Game::HandleEvents()
{
	// Handle quit event
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			gState = QUIT;
		}
		if (event.type == SDL_MOUSEWHEEL)
		{
			if (event.wheel.y > 0)
			{
				//renderer->ChangeCameraZoom(-0.02f);
			}
			else if (event.wheel.y < 0)
			{
				//renderer->ChangeCameraZoom(0.02f);
			}
		}
		if(event.type == SDL_MOUSEBUTTONDOWN && gState == RUNNING)
		{
			switch (event.button.button)
			{
				case SDL_BUTTON_LEFT:
					// Projectile have this weird numbers where the distance is around 42x greater that it actually is
					CreateProjectile(player.GetComponent<TransformComponent>().center, mousePosition, 16800, 300);
					break;
				case SDL_BUTTON_RIGHT:
					// Projectile have this weird numbers where the distance is around 42x greater that it actually is
					if (enemies.size() > 0)
					{
						enemies.front()->Destroy();
					}
					break;
				default:
					break;
			}
		}
	}

	// Handle keyboard input for player movement
	const Uint8* state = SDL_GetKeyboardState(NULL);

	if (gState == RUNNING)
	{
		float moveX = 0.0f;
		float moveY = 0.0f;
		float moveModifier = 100.0f;
		int animSpeedModifier = 100;

		if (state[SDL_SCANCODE_A])
		{
			moveX = -100.0f;
		}
		else if (state[SDL_SCANCODE_D])
		{
			moveX = 100.0f;
		}

		if (state[SDL_SCANCODE_W])
		{
			moveY = -100.0f;
		}
		else if (state[SDL_SCANCODE_S])
		{
			moveY = 100.0f;
		}

		if (state[SDL_SCANCODE_LSHIFT])
		{
			moveModifier = 150.0f;
			animSpeedModifier = 80;
		}
		if (state[SDL_SCANCODE_SPACE] && !keyPress[SDL_SCANCODE_SPACE])
		{
			gState = PAUSED;
			keyPress[SDL_SCANCODE_SPACE] = true;
			//std::cout << "Paused" << "\n";
		}else if (!state[SDL_SCANCODE_SPACE])
		{
			keyPress[SDL_SCANCODE_SPACE] = false;
		}

		// Set player velocity
		player.GetComponent<TransformComponent>().speed = moveModifier;
		player.GetComponent<SpriteComponent>().SetAnimSpeed(animSpeedModifier);
		//std::cout << player.GetComponent<SpriteComponent>().GetAnimSpeed();
		player.GetComponent<TransformComponent>().velocity = glm::vec2(moveX, moveY);

		// Update player animation based on movement direction
		if (moveX < 0.0f) // Moving left
		{
			player.GetComponent<SpriteComponent>().PlayAnimation("Walk2", true);
			player.GetComponent<SpriteComponent>().facing = SpriteComponent::LEFT;
		}
		else if (moveX > 0.0f) // Moving right
		{
			player.GetComponent<SpriteComponent>().PlayAnimation("Walk2");
			player.GetComponent<SpriteComponent>().facing = SpriteComponent::RIGHT;
		}
		else if (moveY < 0.0f) // Moving up
		{
			player.GetComponent<SpriteComponent>().PlayAnimation("Walk3");
			player.GetComponent<SpriteComponent>().facing = SpriteComponent::UP;
		}
		else if (moveY > 0.0f) // Moving down
		{
			player.GetComponent<SpriteComponent>().PlayAnimation("Walk1");
			player.GetComponent<SpriteComponent>().facing = SpriteComponent::DOWN;
		}
		else // Not moving
		{
			if(player.GetComponent<SpriteComponent>().facing == SpriteComponent::LEFT)
				player.GetComponent<SpriteComponent>().PlayAnimation("Idle2", true);

			else if (player.GetComponent<SpriteComponent>().facing == SpriteComponent::RIGHT)
				player.GetComponent<SpriteComponent>().PlayAnimation("Idle2");

			else if (player.GetComponent<SpriteComponent>().facing == SpriteComponent::DOWN)
				player.GetComponent<SpriteComponent>().PlayAnimation("Idle1");

			else if (player.GetComponent<SpriteComponent>().facing == SpriteComponent::UP)
				player.GetComponent<SpriteComponent>().PlayAnimation("Idle3");
		
		}
	}
	else if (gState == PAUSED)
	{
		if (state[SDL_SCANCODE_SPACE] && !keyPress[SDL_SCANCODE_SPACE])
		{
			gState = RUNNING;
			keyPress[SDL_SCANCODE_SPACE] = true;
			//std::cout << "Continued" << "\n";
		}
		else if (!state[SDL_SCANCODE_SPACE])
		{
			keyPress[SDL_SCANCODE_SPACE] = false;
		}
	}
}


void Game::Update()
{
	time->TimeUpdate();
	time->TimeUpdateLate();
	if (gState == RUNNING)
	{
		manager.Refresh();
		manager.Update(time->delta);

		playerHealthBar.GetComponent<TransformComponent>().position = renderer->GetCameraPosition() + glm::vec2(10.0f);
		//playerHealthBar.GetComponent<TransformComponent>().size.x = player.GetComponent<StatComponent>().GetHealthPercent() * playerHealthBar.GetComponent<TransformComponent>().size.x / 256.0f;
		//std::cout << player.GetComponent<StatComponent>().GetHealthPercent() << " | " << playerHealthBar.GetComponent<TransformComponent>().size.x << "\n";
	
		if (enemies.size() < 10)
		{
			glm::vec4 spawnArea;

			spawnArea.x = 128 - renderer->GetCameraPosition().x;
			spawnArea.y = 128 - renderer->GetCameraPosition().y;
			spawnArea.z = 256 + windowWidth;
			spawnArea.w = 256 + windowHeight;

			SpawnEnemy(spawnArea);
		}
		
		CollisionCheck();
		LivingCheck();
	}
	SyncMousePosition();
}

void Game::Draw()
{
	renderer->PreDraw();

	renderer->Draw();
	for (auto& tile : tiles)
	{
		tile->Draw();
	}

	for (auto& mapCollider : mapColliders)
	{
		mapCollider->Draw();
	}

	for (auto& player : players)
	{
		player->Draw();
	}
	for (auto& enemy : enemies)
	{
		enemy->Draw();
	}
	for (auto& projectile : projectiles)
	{
		projectile->Draw();
	}
	for (auto& ui : uis)
	{
		ui->Draw();
	}
	
	SDL_GL_SwapWindow(gWindow);
}

void Game::Clean()
{
	ResourceManager::Clear();

	SDL_DestroyWindow(gWindow);

	SDL_Quit();
}

void Game::SyncMousePosition()
{
	SDL_GetMouseState(&mouseX, &mouseY);
	mousePosition = glm::vec2(mouseX + renderer->GetCameraPosition().x, mouseY + renderer->GetCameraPosition().y);
	//std::cout << mousePosition.x << ", " << mousePosition.y << "\n";
}

void Game::CollisionCheck()
{
	player.GetComponent<ColliderComponent>().SetColor(glm::vec4(1.0f, 1.0f, 0.0f, 0.5f));

	// to setup player collision status and sprite opacity
	if (player.GetComponent<StatComponent>().isInvincible)
	{
		if (SDL_GetTicks() % 250 >= 125)		// Will blink 4 times a second
		{
			player.GetComponent<SpriteComponent>().SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.4f));
		}
		else
		{
			player.GetComponent<SpriteComponent>().SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.6f));
		}
		
	}
	else
	{
		player.GetComponent<SpriteComponent>().SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	}
	

	for (auto& enemy : enemies)
	{
		if (Collision::DetectOnlyCircleAABB(*enemy, player))
		{
			//std::cout << "Collision detected!\n";
			//player.GetComponent<ColliderComponent>().SetColor(glm::vec4(1.0f, 0.0f, 0.0f, 0.75f));
			if (!player.GetComponent<StatComponent>().isInvincible)
			{
				player.GetComponent<StatComponent>().health[0] -= enemy->GetComponent<StatComponent>().attackDamage;
				std::cout << player.GetComponent<StatComponent>().health[0] << "IsAlive: " << player.GetComponent<StatComponent>().isAlive << "\n";
				player.GetComponent<StatComponent>().isInvincible = true;
				player.GetComponent<StatComponent>().lastInvincibility = SDL_GetTicks();
				// WIP
				//player.GetComponent<TransformComponent>().knockbackPosition.x = player.GetComponent<TransformComponent>().position.x + (enemy->GetComponent<TrackerComponent>().direction.x * time->delta * 500);
				//player.GetComponent<TransformComponent>().knockbackPosition.y = player.GetComponent<TransformComponent>().position.y + (enemy->GetComponent<TrackerComponent>().direction.y * time->delta * 500);
				//std::cout << knockbackPos.x << ", " << knockbackPos.y << "\n";
				//player.GetComponent<TransformComponent>().KnockBack(knockbackPos, 0.1f);
				player.GetComponent<TransformComponent>().position.x += enemy->GetComponent<TrackerComponent>().direction.x * time->delta * 500;
				player.GetComponent<TransformComponent>().position.y += enemy->GetComponent<TrackerComponent>().direction.y * time->delta * 500;
			}
			
		}

		for (auto& enemy1 : enemies)
		{
			if (enemy != enemy1)
			{
				CollisionProperty colProp = Collision::ResolutionAABB(*enemy, *enemy1);

				if (std::get<0>(colProp))
				{
					SpriteComponent::Direction direction = std::get<1>(colProp);
					glm::vec2 overlap = std::get<2>(colProp);
					glm::vec2 vecDirection = enemy->GetComponent<TransformComponent>().velocity;
					//For Debugging
					//std::cout << "Direction: " << direction << " | " << "x: " << overlap.x << ", " << overlap.y << "\n";
					//std::cout << "Player Center: " << player.GetComponent<TransformComponent>().center.x << ", " << player.GetComponent<TransformComponent>().center.y << "\n";
					//std::cout << "CollisionBox Center: " << mapCollider->GetComponent<TransformComponent>().center.x << ", " << mapCollider->GetComponent<TransformComponent>().center.y << "\n";


					if (vecDirection.x > 0 && overlap.x < overlap.y)
					{
						enemy->GetComponent<TransformComponent>().position.x -= overlap.x;
					}if (vecDirection.x < 0 && overlap.x < overlap.y)
					{
						enemy->GetComponent<TransformComponent>().position.x += overlap.x;
					}if (vecDirection.y < 0 && overlap.x > overlap.y)
					{
						enemy->GetComponent<TransformComponent>().position.y += overlap.y;
					}if (vecDirection.y > 0 && overlap.x > overlap.y)
					{
						enemy->GetComponent<TransformComponent>().position.y -= overlap.y;
					}
				}
			}
		}

		for (auto& projectile : projectiles)
		{
			if (Collision::AABB(*enemy, *projectile))
			{
				enemy->GetComponent<StatComponent>().health[0] -= player.GetComponent<StatComponent>().attackDamage;
				projectile->Destroy();

				if(!enemy->GetComponent<StatComponent>().isAlive)
				{
					enemy->Destroy();
				}
			}
		}
	}

	for (auto& mapCollider : mapColliders)
	{
		mapCollider->GetComponent<ColliderComponent>().SetColor(glm::vec4(1.0f, 1.0f, 0.0f, 0.5f));

		/*if (Collision::AABB(*mapCollider, player))
		{
			//std::cout << "Collision detected!\n";
			player.GetComponent<ColliderComponent>().SetColor(glm::vec4(1.0f, 0.0f, 1.0f, 0.5f));
			mapCollider->GetComponent<ColliderComponent>().SetColor(glm::vec4(1.0f, 0.0f, 0.0f, 0.5f));
		}*/

		CollisionProperty colProp = Collision::ResolutionAABB(*mapCollider, player);

		if (std::get<0>(colProp))
		{
			SpriteComponent::Direction direction = std::get<1>(colProp);
			glm::vec2 overlap = std::get<2>(colProp);
			glm::vec2 vecDirection = player.GetComponent<TransformComponent>().velocity;
			//For Debugging
			//std::cout << "Direction: " << direction << " | " << "x: " << overlap.x << ", " << overlap.y << "\n";
			//std::cout << "Player Center: " << player.GetComponent<TransformComponent>().center.x << ", " << player.GetComponent<TransformComponent>().center.y << "\n";
			//std::cout << "CollisionBox Center: " << mapCollider->GetComponent<TransformComponent>().center.x << ", " << mapCollider->GetComponent<TransformComponent>().center.y << "\n";


			if (vecDirection.x > 0 && overlap.x < overlap.y)
			{
				player.GetComponent<TransformComponent>().position.x -= overlap.x;
			}if (vecDirection.x < 0 && overlap.x < overlap.y)
			{
				player.GetComponent<TransformComponent>().position.x += overlap.x;
			}if (vecDirection.y < 0 && overlap.x > overlap.y)
			{
				player.GetComponent<TransformComponent>().position.y += overlap.y;
			}if (vecDirection.y > 0 && overlap.x > overlap.y)
			{
				player.GetComponent<TransformComponent>().position.y -= overlap.y;
			}
		}
	}
}

void Game::CreateProjectile(glm::vec2 sourcePosition, glm::vec2 targetPosition, int range, int speed)
{
	auto& projectile(manager.AddEntity());

	glm::vec2 direction = (targetPosition - sourcePosition);
	float m = sqrt((direction.x * direction.x) + (direction.y * direction.y));
	if (m != 0)
	{
		direction.x /= m;
		direction.y /= m;
	}
	projectile.AddComponent<TransformComponent>(sourcePosition, glm::vec2(16.0f, 16.0f), glm::vec2(0.0f), 1, speed);
	projectile.AddComponent<SpriteComponent>(renderer->GetSpriteRenderer(), &ResourceManager::GetTexture("ball"), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
	projectile.AddComponent<ProjectileComponent>(range, speed, direction);
	projectile.AddComponent<ColliderComponent>(renderer->GetSpriteRenderer(), false);
	projectile.AddGroup(Game::groupProjectile);
}

void Game::SpawnEnemy(glm::vec4 spawnArea)
{
	// Generate random point coordinates within spawnArea
	glm::vec2 spawnPoint;
	glm::vec2 cameraPosition = renderer->GetCameraPosition();

	spawnPoint.x = spawnArea.x + static_cast<double>(rand()) / RAND_MAX * ((spawnArea.x + spawnArea.z) - spawnArea.x);
	spawnPoint.y = spawnArea.y + static_cast<double>(rand()) / RAND_MAX * ((spawnArea.y + spawnArea.w) - spawnArea.y);

	// Adjust the coordinates if the point is inside camera
	if (spawnPoint.x >= cameraPosition.x && spawnPoint.x <= (cameraPosition.x + windowWidth) && 
		spawnPoint.y >= cameraPosition.y && spawnPoint.y <= (cameraPosition.y + windowHeight))
	{
		// Shift the point to the right or the ledt, and up or down to ensure its outside the camera area
		spawnPoint.x += (rand() % 2 == 0 ? 1 : -1) * ((cameraPosition.x + windowWidth) - cameraPosition.x);
		spawnPoint.y += (rand() % 2 == 0 ? 1 : -1) * ((cameraPosition.y + windowHeight) - cameraPosition.y);
	}

	auto& enemy(manager.AddEntity());
	enemy.AddComponent<StatComponent>(50, 50, 20);
	enemy.AddComponent<TransformComponent>(glm::vec2(spawnPoint.x, spawnPoint.y), glm::vec2(32.0f, 32.0f), glm::vec2(0.0f), 1, 50);
	enemy.AddComponent<SpriteComponent>(renderer->GetSpriteRenderer(), &ResourceManager::GetTexture("ball"));
	enemy.AddComponent<ColliderComponent>(renderer->GetSpriteRenderer(), false);
	enemy.AddComponent<TrackerComponent>(&player);

	enemy.AddGroup(Game::groupEnemy);
}

void Game::LivingCheck()
{
	if (!player.GetComponent<StatComponent>().isAlive)
	{
		gState = QUIT;
	}
	for (auto& enemy : enemies)
	{
		if (!enemy->GetComponent<StatComponent>().isAlive)
		{
			enemy->Destroy();
		}
	}
}

// WIP
void Game::AlignUI()
{

}