#pragma once

#include "Shader.h"
#include "Texture2D.h"
#include "SpriteSheet.h"

class SpriteRenderer
{
private:
	Shader shader;
	GLuint quadVAO;

	void InitRenderDataIdx();
	void InitRenderData();

public:
	SpriteRenderer(Shader& shader) : shader(shader) { this->InitRenderData(); }
	~SpriteRenderer();

	void DrawSpriteIdx(Texture2D* texture, glm::vec2 position, glm::vec2 size, float rotate = 0.0f, glm::vec4 color = glm::vec4(1.0f));
	void DrawSprite(Texture2D* texture, const glm::vec2& position, const glm::vec2& size, const float& rotate = 0.0f, const glm::vec4& color = glm::vec4(1.0f));
	void DrawSpriteFromSheet(SpriteSheet* spriteSheet, const glm::vec2& texIndex, const glm::vec2& position, const glm::vec2& size, bool horizontalFlip = false, float rotate = 0.0f, const glm::vec4& color = glm::vec4(1.0f));
};