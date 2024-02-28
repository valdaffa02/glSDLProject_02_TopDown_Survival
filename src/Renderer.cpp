#include "Renderer.h"

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include "Math.h"

// Constructor
Renderer::Renderer(const int& windowWidth, const int& windowHeight) : windowWidth(windowWidth), windowHeight(windowHeight), halfWindowWidth(windowWidth/2), halfWindowHeight(windowHeight/2)
{}

// Deconstructor
Renderer::~Renderer(){}

void Renderer::Init()
{
	// Initialize GLEW library
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << "Error: " << glewGetErrorString(err) << std::endl;
		exit(1);
	}
	else
	{
		std::cout << "GLEW Initialized!" << std::endl;
		std::cout << "Version: " << glewGetString(GLEW_VERSION) << std::endl;
	}

	GetOpenGlVersionInfo();

	ResourceManager::LoadShader("shader/vertexSprite.glsl", "shader/fragmentSprite.glsl", nullptr, "spriteShader");
	//ResourceManager::LoadShader("shader/vertexUISprite.glsl", "shader/fragmentUISprite.glsl", nullptr, "uiSpriteShader");
	ResourceManager::LoadShader("shader/vertexQuad.glsl", "shader/fragmentQuad.glsl", nullptr, "quadShader");

	glm::vec2 focusPosition(0.0f);
	camera = new Camera2D(&focusPosition, 1.0f);

	ResourceManager::GetShader("spriteShader").Use().SetInteger("uImage", 0);
	ResourceManager::GetShader("spriteShader").SetMatrix4("uProjection", camera->GetProjectionMatrix(this->windowWidth, this->windowHeight, 0.6f));
	//ResourceManager::GetShader("uiSpriteShader").Use().SetInteger("uImage", 0);
	//ResourceManager::GetShader("uiSpriteShader").SetMatrix4("uProjection", camera->GetProjectionMatrix(this->windowWidth, this->windowHeight, 1.0f));
	//ResourceManager::GetShader("quadShader").SetMatrix4("uProjection", camera->GetProjectionMatrix(this->windowWidth, this->windowHeight));


	// instantiate spriteRenderer & quadRenderer
	spriteRenderer = new SpriteRenderer(ResourceManager::GetShader("spriteShader"));
	//uiSpriteRenderer = new SpriteRenderer(ResourceManager::GetShader("uiSpriteShader"));
	//quadRenderer = new QuadRenderer(ResourceManager::GetShader("quadShader"));
}

void Renderer::PreDraw()
{
	glViewport(0, 0, windowWidth, windowHeight);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	ResourceManager::GetShader("spriteShader").SetMatrix4("uProjection", camera->GetProjectionMatrix(this->windowWidth, this->windowHeight, 1.0f));
	//ResourceManager::GetShader("uiSpriteShader").SetMatrix4("uProjection", camera->GetProjectionMatrix(this->windowWidth, this->windowHeight, 1.0f));

	//std::cout << "Camera Focus: " << camera->focusPosition->x << ", " << camera->focusPosition->y << "\n";
	//std::cout << "Current Camera Focus: " << GetCameraPosition().x << ", " << GetCameraPosition().y << "\n";
	//ResourceManager::GetShader("quadShader").SetMatrix4("uProjection", camera->GetProjectionMatrix(this->windowWidth, this->windowHeight));

	//std::cout << glm::to_string(camera->GetProjectionMatrix(this->windowWidth, this->windowHeight)) << "\n";

}

void Renderer::Draw()
{
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	spriteRenderer->DrawSprite(&ResourceManager::GetTexture("background"), glm::vec2(-800.0f, 0.0f), glm::vec2(this->windowWidth, this->windowHeight), 0.0f);
	//quadRenderer->DrawQuad(glm::vec2(-100.0f, -100.0f), glm::vec2(800.0f, 640.0f), 0.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
}

void Renderer::GetOpenGlVersionInfo()
{
	std::cout << "OpenGL INFO" << std::endl;
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}


void Renderer::SetCameraFocus(glm::vec2* focusPosition)
{
	if (camera != nullptr)
	{
		//cameraFocusPoint = focusPosition;
		camera->focusPosition = focusPosition;
	}
}

void Renderer::ChangeCameraZoom(float value)
{
	if (camera != nullptr)
	{
		if (camera->zoom <= 1.2f && camera->zoom >= 1.0f)
		{
			camera->zoom += value;
		}
		if (camera->zoom < 1.0f)
		{
			camera->zoom = 1.0f;
		}
		if (camera->zoom > 1.2f)
		{
			camera->zoom = 1.2f;
		}
		std::cout << camera->zoom;
	}
}

glm::vec2 Renderer::GetCameraPosition()
{
	if (camera != nullptr)
	{
		return *camera->focusPosition - glm::vec2(halfWindowWidth, halfWindowHeight);
	}
	else
	{
		return glm::vec2(0.0f);
	}
}

glm::mat4x4 Renderer::GetProjection()
{
	return camera->GetProjectionMatrix(this->windowWidth, this->windowHeight, 1.0f);
}


// Error Handling Functions
void Renderer::GLClearAllErrors()
{
	while (glGetError() != GL_NO_ERROR){}
}

// Returns true if there's an error
bool Renderer::GLCheckErrorStatus(const char* function, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "OpenGL Error: " << error << "\tLine: " << line << "\tfunction: " << function << std::endl;
		return true;
	}
	return false;
}