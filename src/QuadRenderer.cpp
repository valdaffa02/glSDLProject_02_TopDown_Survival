#include "QuadRenderer.h"

#include "glm/gtc/matrix_transform.hpp"


#include <iostream>

void QuadRenderer::InitRenderDataIdx()
{
	GLuint VBO;
	GLuint EBO;
	GLfloat vertices[] = {
		// pos (x, y)
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f
	};

	GLuint indices[] = {
		0, 2, 3,
		1, 0, 3
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

void QuadRenderer::InitRenderData()
{
	GLuint VBO;
	GLuint EBO;
	GLfloat vertices[] =
	{
		// pos (x, y)
		0.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f
	};

	glGenVertexArrays(1, &this->quadVAO);

	// Setup VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	glBindVertexArray(this->quadVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

QuadRenderer::~QuadRenderer() {}

void QuadRenderer::DrawQuadIdx(glm::vec2 position, glm::vec2 size, float rotate, glm::vec4 color)
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


	this->shader.SetMatrix4("uModel", model);
	this->shader.SetVector4f("uColor", color);
	//glm::vec4 uCol = this->shader.GetUniformValue4f("uColor");

	//std::cout << uCol.x << ", " << uCol.y << ", " << uCol.z << ", " << uCol.w << "\n";

	glBindVertexArray(this->quadVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}

void QuadRenderer::DrawQuad(glm::vec2 position, glm::vec2 size, float rotate, glm::vec4 color)
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


	this->shader.SetMatrix4("uModel", model);
	this->shader.SetVector4f("uColor", color);
	glm::vec4 uCol = this->shader.GetUniformValue4f("uColor");

	//std::cout << uCol.x << ", " << uCol.y << ", " << uCol.z << ", " << uCol.w << "\n";

	glBindVertexArray(this->quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}
