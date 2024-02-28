#pragma once
#define GLEW_STATIC

#include <glew.h>
#include <gl/glu.h>

#include "ResourceManager.h"
#include "Camera2D.h"

#include "SpriteRenderer.h"
#include "QuadRenderer.h"

// for error checking
#define GLCheck(x) GLClearAllErrors(); x; GLCheckErrorStatus(#x, __LINE__);

class Renderer
{
private:
	// Window properties
	int windowWidth;
	int windowHeight;
	int halfWindowWidth;
	int halfWindowHeight;

	// VAO
	GLuint vertexArrayObject = 0;
	// VBO
	GLuint vertexBufferObject = 0;
	// EBO
	GLuint elementBufferObject = 0;

	// Camera
	glm::vec2* cameraFocusPoint;
	Camera2D* camera;




	// Renderer
	SpriteRenderer* spriteRenderer;
	// Experimental Renderer
	SpriteRenderer* uiSpriteRenderer;
	QuadRenderer* quadRenderer;

public:
	// Constructor
	Renderer(const int& windowWidth, const int& windowHeight);

	// Deconstructor
	~Renderer();

	void Init();

	void PreDraw();

	void Draw();

	void GetOpenGlVersionInfo();

	// Camera stuff
	void SetCameraFocus(glm::vec2* focusPosition);
	void ChangeCameraZoom(float value);
	glm::vec2 GetCameraPosition();
	
	// Retrieve projection
	glm::mat4x4 GetProjection();

	// Retrieve sprite renderer
	SpriteRenderer* GetSpriteRenderer() { return spriteRenderer; }
	SpriteRenderer* GetUISpriteRenderer() { return uiSpriteRenderer; }
	QuadRenderer* GetQuadRenderer() { return quadRenderer; }

	

	// Error Handling Functions
	static void GLClearAllErrors();

	// Returns true if there's an error
	static bool GLCheckErrorStatus(const char* function, int line);
};