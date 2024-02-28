#pragma once

#include <string>

#include <glew.h>
#include <gl/glu.h>

#include <glm/glm.hpp>



class Shader
{
private:
	void CheckCompileErrors(GLuint object, std::string type);


public:
	// Shader ID
	GLuint ID;

	// Constructor
	Shader(){}

	// Deconstructor
	~Shader(){}

	// Set current shader as active
	Shader& Use();

	// Compilator
	void Compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource);

	// Utility function for uniforms
	void SetFloat(const char* name, float value, bool useShader = false);
	void SetInteger(const char* name, int value, bool useShader = false);
	void SetVector2f(const char* name, float x, float y, bool useShader = false);
	void SetVector2f(const char* name, const glm::vec2& value, bool useShader = false);
	void SetVector3f(const char* name, float x, float y, float z, bool useShader = false);
	void SetVector3f(const char* name, const glm::vec3& value, bool useShader = false);
	void SetVector4f(const char* name, float x, float y, float z, float w, bool useShader = false);
	void SetVector4f(const char* name, const glm::vec4& value, bool useShader = false);
	void SetMatrix4(const char* name, const glm::mat4& matrix, bool useShader = false);
	glm::vec4 GetUniformValue4f(const char* name);

};