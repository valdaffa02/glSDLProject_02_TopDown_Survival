#pragma once

#include "Components.h"
#include "glm/glm.hpp"

class ProjectileComponent : public Component
{
private:
	int range;
	int speed;
	int distance;
	glm::vec2 velocity;

	TransformComponent* transform;

public:
	ProjectileComponent(int range, int speed, glm::vec2 velocity) : range(range), speed(speed), velocity(velocity)
	{}

	~ProjectileComponent(){}

	void Init() override
	{
		distance = 0;
		/*
		if (!entity->HasComponent<TransformComponent>())
		{
			entity->AddComponent<TransformComponent>();
		}*/

		transform = &entity->GetComponent<TransformComponent>();
		transform->velocity = velocity;
	}

	void Update(float& deltaTime) override
	{
		distance += speed;

		if (distance > range)
		{
			entity->Destroy();
		}
	}
};