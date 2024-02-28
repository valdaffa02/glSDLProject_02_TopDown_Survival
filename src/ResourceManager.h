#pragma once

#include <map>
#include <string>


#include "Shader.h"
#include "Texture2D.h"
#include "SpriteSheet.h"



class ResourceManager
{
private:
	

	// Loads and generates a shader from file
	static Shader LoadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);

	// Liads a single texture from file
	static Texture2D LoadTextureFromFile(const char* file, bool alpha);

	static SpriteSheet LoadSpriteSheetFromFile(const char* file, bool alpha, int texWidth, int texHeight);
	
	ResourceManager();
	~ResourceManager();

public:
	// Resource Storage
	static std::map<std::string, Shader> shaders;
	static std::map<std::string, Texture2D> textures;
	static std::map<std::string, SpriteSheet> spriteSheets;

	

	// Shader Management
	// Load Shader
	static Shader LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name);
	// Retrieves stored shader
	static Shader& GetShader(std::string name);
	
	//Texture Management
	// Load Texture
	static Texture2D LoadTexture(const char* file, bool alpha, std::string name);

	static Texture2D& GetTexture(std::string name);

	// Load SpriteSheet
	static SpriteSheet LoadSpriteSheet(const char* file, bool alpha, std::string name, int texWidth, int texHeight);

	static SpriteSheet& GetSpriteSheet(std::string name);

	// De-allocates all loaded resources
	static void Clear();
};