#pragma once


#include "Components.h"
#include "../SpriteRenderer.h"
#include "../ResourceManager.h"

#include "glm/glm.hpp"


class ColliderComponent : public Component
{
public:
	glm::vec2 position;
	glm::vec2 size;
	glm::vec4 color;

	bool isDynamic;

	TransformComponent* transform;
	SpriteRenderer* renderer;

	ColliderComponent()
	{}

	ColliderComponent(SpriteRenderer* renderer, bool isDynamic = false) : renderer(renderer), isDynamic(isDynamic)
	{}

	ColliderComponent(SpriteRenderer* renderer, const glm::vec2& position, const glm::vec2& size, const bool& isDynamic = false) : renderer(renderer), position(position), size(size), isDynamic(isDynamic)
	{}

	void Init() override
	{
		if (!entity->HasComponent<TransformComponent>())
		{
			entity->AddComponent<TransformComponent>(position, size);
		}

		transform = &entity->GetComponent<TransformComponent>();
		color = glm::vec4(1.0f, 1.0f, 0.0f, 0.5f);
	}

	void Update(float& deltaTime)
	{
		this->position = transform->position;
		if (!isDynamic)
		{
			this->size = transform->size;
		}
	}


	void Draw()
	{
		renderer->DrawSprite(&ResourceManager::GetTexture("collisionBox"), this->position, this->size, 0.0f, color);
		//std::cout <<  <<"\n";
	}

	void SetColor(const glm::vec4& color)
	{
		this->color = color;
	}
};