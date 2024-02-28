#pragma once

#include "Shader.h"

class QuadRenderer
{
private:
	Shader shader;
	GLuint quadVAO;

	void InitRenderDataIdx();
	void InitRenderData();

public:
	QuadRenderer(Shader& shader) : shader(shader) { this->InitRenderData(); }
	~QuadRenderer();

	void DrawQuadIdx(glm::vec2 position, glm::vec2 size, float rotate = 0.0f, glm::vec4 color = glm::vec4(1.0f));
	void DrawQuad(glm::vec2 position, glm::vec2 size, float rotate = 0.0f, glm::vec4 color = glm::vec4(1.0f));
};