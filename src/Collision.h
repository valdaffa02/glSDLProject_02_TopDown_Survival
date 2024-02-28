#pragma once

#include "ECS/Components.h"

#include "glm/glm.hpp"

#include <cmath>

typedef std::tuple<bool, SpriteComponent::Direction, glm::vec2> CollisionProperty;


class Collision
{
public:
	static bool AABB(Entity& one, Entity& two)
	{
		bool collisionX = (one.GetComponent<TransformComponent>().position.x + one.GetComponent<TransformComponent>().size.x >= two.GetComponent<TransformComponent>().position.x) &&
			(two.GetComponent<TransformComponent>().position.x + two.GetComponent<TransformComponent>().size.x >= one.GetComponent<TransformComponent>().position.x);

		bool collisionY = (one.GetComponent<TransformComponent>().position.y + one.GetComponent<TransformComponent>().size.y >= two.GetComponent<TransformComponent>().position.y) &&
			(two.GetComponent<TransformComponent>().position.y + two.GetComponent<TransformComponent>().size.y >= one.GetComponent<TransformComponent>().position.y);

		return (collisionX && collisionY);
	}

	static CollisionProperty ResolutionAABB(Entity& one, Entity& two)
	{
		if (AABB(one, two))
		{
			glm::vec2 overlap;
			overlap.x = std::abs(std::min(one.GetComponent<TransformComponent>().position.x + one.GetComponent<TransformComponent>().size.x, // get minimum of the right most of both entities to get the most right of the overlap
				two.GetComponent<TransformComponent>().position.x + two.GetComponent<TransformComponent>().size.x)					// get maximum of the left most of both entities to get the most left of the overlap
				- std::max(one.GetComponent<TransformComponent>().position.x, two.GetComponent<TransformComponent>().position.x));	// then substract both said minimum with the maximum to get the width of the overlap
			overlap.y = std::abs(std::min(one.GetComponent<TransformComponent>().position.y + one.GetComponent<TransformComponent>().size.y, // get minimum of the bottom most of both entities to get the most bottom of the overlap
				two.GetComponent<TransformComponent>().position.y + two.GetComponent<TransformComponent>().size.y)					// get maximum of the top most of both entities to get the most top of the overlap
				- std::max(one.GetComponent<TransformComponent>().position.y, two.GetComponent<TransformComponent>().position.y));	// then substract both said minimum with the maximum to get the height of the overlap

			glm::vec2 direction;
			direction.x = one.GetComponent<TransformComponent>().center.x - two.GetComponent<TransformComponent>().center.x;
			direction.y = one.GetComponent<TransformComponent>().center.y - two.GetComponent<TransformComponent>().center.y;
			// normalize the direction
			float m = sqrt((direction.x * direction.x) + (direction.y * direction.y));
			if (m != 0)
			{
				direction.x /= m;
				direction.y /= m;
			}

			

			float angle = std::atan2(direction.y, direction.x);

			angle = glm::degrees(angle);

			SpriteComponent::Direction enumDirection;

			if (angle >= -45 && angle < 45)
			{
				enumDirection = SpriteComponent::LEFT;
			}
			else if (angle >= 45 && angle < 135)
			{
				enumDirection = SpriteComponent::UP;
			}
			else if (angle >= -135 && angle < -45)
			{
				enumDirection = SpriteComponent::DOWN;
			}
			else
			{
				enumDirection = SpriteComponent::RIGHT;
			}

			return std::make_tuple(true, enumDirection, overlap);
		}

		return std::make_tuple(false, SpriteComponent::UP, glm::vec2(0.0f, 0.0f));
	}

	static float Clamp(float value, float min, float max) { return std::max(min, std::min(max, value)); }

	/*static CollisionProperty CircleAABB(Entity& ball, Entity& rectangle)
	{
		// calculate half extents and center of rectangle
		glm::vec2 rectangleHalfExtents(rectangle.GetComponent<TransformComponent>().size.x / 2.0f, rectangle.GetComponent<TransformComponent>().size.y / 2.0f);
		glm::vec2 rectangleCenter(rectangle.GetComponent<TransformComponent>().position.x + rectangleHalfExtents.x, rectangle.GetComponent<TransformComponent>().position.y + rectangleHalfExtents.y);

		// get difference vector between both center
		glm::vec2 difference = ball.GetComponent<TransformComponent>().center - rectangle.GetComponent<TransformComponent>().center;
		glm::vec2 clamped(Clamp(difference.x, -rectangleHalfExtents.x, rectangleHalfExtents.x), Clamp(difference.y, -rectangleHalfExtents.y, rectangleHalfExtents.y));

		// add clamp value to rectangleCenter to get the value of point closest to circle
		glm::vec2 closest = rectangleCenter + clamped;

		difference = closest - ball.GetComponent<TransformComponent>().center;
		if (glm::length(difference) <= (ball.GetComponent<TransformComponent>().size.x / 2))		// return true if length of difference smaller than ball radius (if the closest point intersect with ball)
			return std::make_tuple(true, ball.GetComponent<TransformComponent>().VectorDirection(difference), difference);
		else
			return std::make_tuple(false, SpriteComponent::UP, glm::vec2(0.0f, 0.0f));
	}*/

	static bool DetectOnlyCircleAABB(Entity& ball, Entity& rectangle)
	{
		// calculate half extents and center of rectangle
		glm::vec2 rectangleHalfExtents(rectangle.GetComponent<TransformComponent>().size.x / 2.0f, rectangle.GetComponent<TransformComponent>().size.y / 2.0f);
		glm::vec2 rectangleCenter(rectangle.GetComponent<TransformComponent>().position.x + rectangleHalfExtents.x, rectangle.GetComponent<TransformComponent>().position.y + rectangleHalfExtents.y);

		// get difference vector between both center
		glm::vec2 difference = ball.GetComponent<TransformComponent>().center - rectangle.GetComponent<TransformComponent>().center;
		glm::vec2 clamped(Clamp(difference.x, -rectangleHalfExtents.x, rectangleHalfExtents.x), Clamp(difference.y, -rectangleHalfExtents.y, rectangleHalfExtents.y));

		// add clamp value to rectangleCenter to get the value of point closest to circle
		glm::vec2 closest = rectangleCenter + clamped;

		difference = closest - ball.GetComponent<TransformComponent>().center;
		return glm::length(difference) <= (ball.GetComponent<TransformComponent>().size.x / 2); // return true if length of difference smaller than ball radius (if the closest point intersect with ball)
	}
};