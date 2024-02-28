#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>

#include "Math.h"


class Camera2D
{
public:
	glm::vec2* focusPosition;
	glm::vec2 currentPosition;
	float zoom;

	Camera2D(){}

	Camera2D(glm::vec2* focusPosition, float zoom) : focusPosition(focusPosition), zoom(zoom), currentPosition(*focusPosition)
	{}

	glm::mat4x4 GetProjectionMatrix(const int& windowWidth, const int& windowHeight, const float& factor)
	{
		// Calculate viewport width and height in world space
		float viewportWidth = windowWidth / zoom;
		float viewportHeight = windowHeight / zoom;

		// Calculate adjusted left, right, top, and bottom boundaries
		/*
		float left = focusPosition->x - viewportWidth / 2.0f;
		float right = focusPosition->x + viewportWidth / 2.0f;
		float top = focusPosition->y - viewportHeight / 2.0f;
		float bottom = focusPosition->y + viewportHeight / 2.0f;
		*/
		currentPosition = Math::LinearInterpolation2F(currentPosition, *focusPosition, factor);

		float left = currentPosition.x - viewportWidth / 2.0f;
		float right = currentPosition.x + viewportWidth / 2.0f;
		float top = currentPosition.y - viewportHeight / 2.0f;
		float bottom = currentPosition.y + viewportHeight / 2.0f;


		//glm::mat4 orthoMatrix = glm::ortho(left + 100, right - 100, bottom - 80, top + 80, -1.0f, 1.0f);
		//std::cout << left << " | " << right << " | " << bottom << " | " << top << "\n";
		glm::mat4 orthoMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);

		return orthoMatrix;
	}

};