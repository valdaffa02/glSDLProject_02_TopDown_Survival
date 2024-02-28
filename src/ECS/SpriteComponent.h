#pragma once

#include "glm/glm.hpp"

#include "Components.h"
#include "../Texture2D.h"

#include "../SpriteRenderer.h"
#include "Animation.h"

#include <map>

class SpriteComponent : public Component
{
private:
	TransformComponent* transform = nullptr;
	Texture2D* texture = nullptr;
	SpriteSheet* spriteSheet = nullptr;
	glm::vec4 color;

	SpriteRenderer* renderer;
	float rotation;

	bool horizontalFlip;
	

	// For sprite animation
	bool animated;
	int animIndex;
	int animFrames;
	int currentAnimFrame;
	int animSpeed;

	std::map<std::string, Animation*> animations;

public:

	enum Direction
	{
		LEFT = 0,
		DOWN = 1,
		RIGHT = 2,
		UP = 3
	};

	Direction facing;
	
	SpriteComponent() = default;

	SpriteComponent(SpriteRenderer* renderer, Texture2D* texture, glm::vec4 color = glm::vec4(1.0), float rotation = 0.0f, bool animated = false, int animIndex = 0, int animFrames = 0, int animSpeed = 0) : renderer(renderer), texture(texture), color(color), rotation(rotation), animated(animated), animIndex(animIndex), animFrames(animFrames), animSpeed(animSpeed)
	{
		if (animated)
		{
			// for animation
		}
	}

	SpriteComponent(SpriteRenderer* renderer, SpriteSheet* spriteSheet, glm::vec4 color = glm::vec4(1.0), float rotation = 0.0f, bool animated = false, int animIndex = 0, int animFrames = 0, int animSpeed = 0) : renderer(renderer), spriteSheet(spriteSheet), color(color), rotation(rotation), animated(animated), animIndex(animIndex), animFrames(animFrames), animSpeed(animSpeed)
	{
		if (animated)
		{
			// for animation
		}
	}

	~SpriteComponent() {}

	void Init() override
	{
		transform = &entity->GetComponent<TransformComponent>();

		horizontalFlip = false;
		facing = DOWN;
		/*if (renderer != nullptr)
		{
			std::cout << "Renderer isn't null" << std::endl;
		}*/
	}

	void Update(float& deltaTime) override
	{
		if (animated)
		{	// for animation
			if (!animations.empty())
			{
				currentAnimFrame = static_cast<int>(SDL_GetTicks() / animSpeed) % animFrames;
				//std::cout << animSpeed << "\n";
			}
			
		}
	}

	void Draw() override
	{
		/*std::cout << "position: " << this->transform->position.x << ", " << this->transform->position.y << " | " <<
			"size: " << (this->transform->size.x * this->transform->scale) << ", " << (this->transform->size.y * this->transform->scale) << " | " <<
			"rotation: " << this->rotation << " | " <<
			"color: " << this->color.r << ", " << this->color.g << ", " << this->color.b <<
			std::endl;*/
		if (animated)
		{
			if (!animations.empty())
			{
				renderer->DrawSpriteFromSheet(this->spriteSheet, glm::vec2(currentAnimFrame, animIndex), this->transform->position, (this->transform->size * this->transform->scale), this->horizontalFlip, this->rotation, this->color);
				//if (horizontalFlip)
					//std::cout << "Flipped!\n";
			}
			else
			{
				renderer->DrawSpriteFromSheet(this->spriteSheet, glm::vec2(0.0f), this->transform->position, (this->transform->size * this->transform->scale), this->horizontalFlip, this->rotation, this->color);
			}
			
		}
		else
		{
			renderer->DrawSprite(this->texture, this->transform->position, (this->transform->size * this->transform->scale), this->rotation, this->color);
		}
		
	}


	void AddAnimation(std::string id, int index, int frames, int speed)
	{
		Animation* newAnim = new Animation(index, frames, speed);
		animations.emplace(id, newAnim);
	}

	void PlayAnimation(std::string id, bool horizontalFlip = false)
	{
		if (animations.find(id) != animations.end())
		{
			animFrames = animations[id]->frames;
			animIndex = animations[id]->index;
			//animSpeed = animations[id]->speed;
			this->horizontalFlip = horizontalFlip;
		}
	}

	void SetAnimSpeed(const int& animSpeed)
	{
		this->animSpeed = animSpeed;
	}

	int GetAnimSpeed()
	{
		return animSpeed;
	}

	void SetColor(const glm::vec4& color)
	{
		this->color = color;
	}
};