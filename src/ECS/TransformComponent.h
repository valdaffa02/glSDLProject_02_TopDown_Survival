#pragma once

#include <glm/glm.hpp>
#include "../Math.h"

#include "Components.h"


class TransformComponent : public Component
{
public:
	glm::vec2 position;
	glm::vec2 size;
	glm::vec2 center;
	glm::vec2 velocity;

	// WIP
	glm::vec2 knockbackPosition;

	float scale;

	int speed;

	TransformComponent()
	{}

	TransformComponent(glm::vec2 position, glm::vec2 size, glm::vec2 velocity = glm::vec2(0.0f, 0.0f), float scale = 1, int speed = 0) : position(position), size(size), velocity(velocity), scale(scale), speed(speed)
	{
		center.x = position.x + (size.x / 2);
		center.y = position.y + (size.y / 2);
	}

	void Init() override
	{
		if (entity->HasComponent<StatComponent>())
		{
			speed = entity->GetComponent<StatComponent>().speed;
		}
		knockbackPosition = position;
	}

	void Update(float& deltaTime) override
	{
		// Update position
		// normalize velocity
		//std::cout << "same\n";

		float m = sqrt((velocity.x * velocity.x) + (velocity.y * velocity.y));
		if (m != 0)
		{
			velocity.x /= m;
			velocity.y /= m;
		}

		position.x += velocity.x * deltaTime * speed;
		position.y += velocity.y * deltaTime * speed;
		center.x = position.x + (size.x / 2);
		center.y = position.y + (size.y / 2);

		
		
		//std::cout << "Position X: " << position.x << " =  " << velocity.x << "*" << deltaTime << "*" << speed << "\n";
		//std::cout << "Position Y: " << position.y << " =  " << velocity.y << "*" << deltaTime << "*" << speed << "\n";
	}

	// WIP
	void KnockBack(const float& factor)
	{
		//std::cout << "Interpolation value: " << Math::LinearInterpolation2F(position, knockPosition, factor).x << ", " << Math::LinearInterpolation2F(position, targetPosition, factor).y << "\n";
		position = Math::LinearInterpolation2F(position, knockbackPosition, factor);
		std::cout << "Interpolated position: " << position.x << ", " << position.y << "\n";
	}
};