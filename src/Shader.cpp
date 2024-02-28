#include "Shader.h"

#include "glm/gtc/type_ptr.hpp"
#include <iostream>

// Set current shader as active
Shader& Shader::Use()
{
	glUseProgram(this->ID);
	return *this;
}

// Compilator
void Shader::Compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource)
{
	GLuint vertexShaderObj, fragmentShaderObj, geometryShaderObj;

	// VertexShader
	vertexShaderObj = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderObj, 1, &vertexSource, NULL);
	glCompileShader(vertexShaderObj);
	// Check for error
	CheckCompileErrors(vertexShaderObj, "VERTEX");

	// FragmentShader
	fragmentShaderObj = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderObj, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShaderObj);
	// Check for error
	CheckCompileErrors(fragmentShaderObj, "FRAGMENT");

	// If geometry shader included
	if (geometrySource != nullptr)
	{
		// GeometryShader
		geometryShaderObj = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometryShaderObj, 1, &geometrySource, NULL);
		glCompileShader(geometryShaderObj);
		// Check for error
		CheckCompileErrors(geometryShaderObj, "GEOMETRY");
	}

	// Create Shader Program
	this->ID = glCreateProgram();
	glAttachShader(this->ID, vertexShaderObj);
	glAttachShader(this->ID, fragmentShaderObj);
	if (geometrySource != nullptr)
		glAttachShader(this->ID, geometryShaderObj);
	glLinkProgram(this->ID);
	CheckCompileErrors(this->ID, "PROGRAM");

	// Delete the shaders after being linked
	glDeleteShader(vertexShaderObj);
	glDeleteShader(fragmentShaderObj);
	if (geometrySource != nullptr)
		glDeleteShader(geometryShaderObj);
}



// Util Functions
void Shader::SetFloat(const char* name, float value, bool useShader)
{
	if (useShader)
		this->Use();
	glUniform1f(glGetUniformLocation(this->ID, name), value);
}
void Shader::SetInteger(const char* name, int value, bool useShader)
{
	if (useShader)
		this->Use();
	glUniform1i(glGetUniformLocation(this->ID, name), value);
}
void Shader::SetVector2f(const char* name, float x, float y, bool useShader)
{
	if (useShader)
		this->Use();
	glUniform2f(glGetUniformLocation(this->ID, name), x, y);
}
void Shader::SetVector2f(const char* name, const glm::vec2& value, bool useShader)
{
	if (useShader)
		this->Use();
	glUniform2f(glGetUniformLocation(this->ID, name), value.x, value.y);
}
void Shader::SetVector3f(const char* name, float x, float y, float z, bool useShader)
{
	if (useShader)
		this->Use();
	glUniform3f(glGetUniformLocation(this->ID, name), x, y, z);
}
void Shader::SetVector3f(const char* name, const glm::vec3& value, bool useShader)
{
	if (useShader)
		this->Use();
	glUniform3f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z);
}
void Shader::SetVector4f(const char* name, float x, float y, float z, float w, bool useShader)
{
	if (useShader)
		this->Use();
	glUniform4f(glGetUniformLocation(this->ID, name), x, y, z, w);
}
void Shader::SetVector4f(const char* name, const glm::vec4& value, bool useShader)
{
	if (useShader)
		this->Use();
	glUniform4f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z, value.w);
}
void Shader::SetMatrix4(const char* name, const glm::mat4& matrix, bool useShader)
{
	if (useShader)
		this->Use();
	glUniformMatrix4fv(glGetUniformLocation(this->ID, name), 1, false, glm::value_ptr(matrix));
}


// Error Checking function
void Shader::CheckCompileErrors(GLuint object, std::string type)
{
	// Retrieve the result of the compilation
	int result;
	// Retrieve the compilation status
	if (type != "PROGRAM")
	{
		glGetShaderiv(object, GL_COMPILE_STATUS, &result);

		if (result == GL_FALSE)
		{
			int length;
			glGetShaderiv(object, GL_INFO_LOG_LENGTH, &length);
			char* errorMessages = new char[length];
			glGetShaderInfoLog(object, length, &length, errorMessages);


			std::cout << "ERROR: GL_" << type << "_SHADER compilation failed!\n" << errorMessages << std::endl;


			// Reclaim the memory
			delete[] errorMessages;

			// Delete the broken shader
			glDeleteShader(object);
		}
	}
	else
	{
		glGetProgramiv(object, GL_LINK_STATUS, &result);
		if (result == GL_FALSE)
		{
			int length;
			glGetProgramiv(object, GL_INFO_LOG_LENGTH, &length);
			char* errorMessages = new char[length];
			glGetProgramInfoLog(object, length, &length, errorMessages);

			std::cout << "ERROR: GL_" << type << "_SHADER Linking failed!\n" << errorMessages << std::endl;

			// Reclaim the memory
			delete[] errorMessages;

			// Delete the broken program
			glDeleteProgram(object);
		}
	}

}

glm::vec4 Shader::GetUniformValue4f(const char* name)
{
	float f[4] = { 0.0f };
	glGetnUniformfv(this->ID, glGetUniformLocation(this->ID, name), 4*sizeof(float), f);

	glm::vec4 value = { f[0], f[1], f[2], f[3]};

	return value;
}