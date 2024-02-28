#pragma once

#include "Components.h"
#include "glm/glm.hpp"

class TrackerComponent : public Component
{
public:
	TransformComponent* transform;

	glm::vec2* targetPosition;
	Entity* target;

	glm::vec2 direction;

	TrackerComponent(Entity* target)
	{
		this->target = target;
	}

	~TrackerComponent()
	{}

	void Init() override
	{
		transform = &entity->GetComponent<TransformComponent>();
		direction = glm::vec2(0.0f, 0.0f);

		targetPosition = &target->GetComponent<TransformComponent>().center;
	}

	void Update(float& deltaTime) override
	{
		if (transform->position.x == targetPosition->x && transform->position.y == targetPosition->y)
		{
			std::cout << "Same spot!\n";
		}
		direction.x = targetPosition->x - transform->position.x;
		direction.y = targetPosition->y - transform->position.y;
		float m = sqrt((direction.x * direction.x) + (direction.y * direction.y));
		if (m != 0)
		{
			direction.x /= m;
			direction.y /= m;
		}

		transform->velocity = direction;
	}
};