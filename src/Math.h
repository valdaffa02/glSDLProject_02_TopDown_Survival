#pragma once

#include "glm/glm.hpp"

struct Math
{
	static glm::vec2 LinearInterpolation2F(glm::vec2& startPoint, glm::vec2& endPoint, const float& factor = 1.0f)
	{
		return glm::vec2(startPoint.x * (1.0 - factor) + endPoint.x * factor, startPoint.y * (1.0 - factor) + endPoint.y * factor);
	}

	static float LinearInterpolationF(float& startPoint, float& endPoint, const float& factor = 1.0f)
	{
		return startPoint * (1.0 - factor) + endPoint * factor;
	}
};
