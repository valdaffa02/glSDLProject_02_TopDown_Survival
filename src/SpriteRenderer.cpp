#include "SpriteRenderer.h"

#include "glm/gtc/matrix_transform.hpp"


#include <iostream>

void SpriteRenderer::InitRenderDataIdx()
{
	GLuint VBO;
	GLuint EBO;
	GLfloat vertices[] = {
		// pos (x, y)		// tex
		0.0f, 0.0f,			0.0f, 0.0f,
		1.0f, 0.0f,			1.0f, 0.0f,
		0.0f, 1.0f,			0.0f, 1.0f,
		1.0f, 1.0f,			1.0f, 1.0f
	};

	GLuint indices[] = {
		2, 0, 1,
		3, 2, 1
	};

	glGenVertexArrays(1, &this->quadVAO);
	
	// Setup VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);



	glBindVertexArray(this->quadVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	// Setup EBO
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
}


void SpriteRenderer::InitRenderData()
{
	// configure VAO/VBO
	GLuint VBO;
	GLfloat vertices[] =
	{
		// pos (x, y)		// tex
		0.0f, 1.0f,		0.0f, 1.0f,
		1.0f, 0.0f,		1.0f, 0.0f,
		0.0f, 0.0f,		0.0f, 0.0f,

		0.0f, 1.0f,		0.0f, 1.0f,
		1.0f, 1.0f,		1.0f, 1.0f,
		1.0f, 0.0f,		1.0f, 0.0f
	};


	glGenVertexArrays(1, &this->quadVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(this->quadVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

SpriteRenderer::~SpriteRenderer(){}

void SpriteRenderer::DrawSpriteIdx(Texture2D* texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec4 color)
{
	this->shader.Use();
	glm::mat4 model = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	model = glm::translate(model, glm::vec3(position, 0.0f));

	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
	model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

	glm::vec4 textureOffset(1.0f);

	this->shader.SetMatrix4("uModel", model);
	this->shader.SetVector4f("uSpriteColor", color);
	this->shader.SetVector4f("uTextureOffset", textureOffset);

	glActiveTexture(GL_TEXTURE0);
	texture->Bind();

	glBindVertexArray(this->quadVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}


void SpriteRenderer::DrawSprite(Texture2D* texture, const glm::vec2& position, const glm::vec2& size, const float& rotate, const glm::vec4& color)
{
	this->shader.Use();
	glm::mat4 model = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	model = glm::translate(model, glm::vec3(position, 0.0f));

	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
	model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

	model = glm::scale(model, glm::vec3(size, 1.0f));

	// x is horizontal index, y is vertical index, z is normalized width (0 - 1), w is normalized height (0 - 1)
	glm::vec4 textureOffset(1.0f);

	this->shader.SetMatrix4("uModel", model);
	this->shader.SetVector4f("uSpriteColor", color);
	this->shader.SetVector4f("uTextureOffset", textureOffset);
	this->shader.SetInteger("uHorizontalFlip", 0);

	glActiveTexture(GL_TEXTURE0);
	texture->Bind();

	glBindVertexArray(this->quadVAO);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}


void SpriteRenderer::DrawSpriteFromSheet(SpriteSheet* spriteSheet, const glm::vec2& texIndex, const glm::vec2& position, const glm::vec2& size, bool horizontalFlip, float rotate, const glm::vec4& color)
{
	this->shader.Use();
	glm::mat4 model = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	model = glm::translate(model, glm::vec3(position, 0.0f));

	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
	model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

	model = glm::scale(model, glm::vec3(size, 1.0f));

	// x is horizontal index, y is vertical index, z is normalized width (0 - 1), w is normalized height (0 - 1)
	glm::vec4 textureOffset = { texIndex.x, texIndex.y, (static_cast<float>(spriteSheet->textureWidth) / spriteSheet->width), (static_cast<float>(spriteSheet->textureHeight) / spriteSheet->height) };

	int iHorizontalFlip = 0;
	if (horizontalFlip)
	{
		iHorizontalFlip = 1;
		//std::cout << "FLIPPED!\n";
	}
		
	
	this->shader.SetMatrix4("uModel", model);
	this->shader.SetVector4f("uSpriteColor", color);
	this->shader.SetVector4f("uTextureOffset", textureOffset);
	this->shader.SetInteger("uHorizontalFlip", iHorizontalFlip);
	

	glActiveTexture(GL_TEXTURE0);
	spriteSheet->Bind();

	glBindVertexArray(this->quadVAO);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);


}
	
