#include "ResourceManager.h"

#include <iostream>
#include <sstream>
#include <fstream>


#include <SDL_image.h>

ResourceManager::ResourceManager() {}
ResourceManager::~ResourceManager() {}

// Instantiate static variables
std::map<std::string, Texture2D> ResourceManager::textures;
std::map<std::string, Shader> ResourceManager::shaders;
std::map<std::string, SpriteSheet> ResourceManager::spriteSheets;

// Shader Management
// Load Shader
Shader ResourceManager::LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name)
{
	shaders[name] = LoadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
	return shaders[name];
}
// Retrieves stored shader
Shader& ResourceManager::GetShader(std::string name)
{
	return shaders[name];
}

//Texture Management
// Load Texture
Texture2D ResourceManager::LoadTexture(const char* file, bool alpha, std::string name)
{
	textures[name] = LoadTextureFromFile(file, alpha);
	return textures[name];
}
// Retrieves stored texture
Texture2D& ResourceManager::GetTexture(std::string name)
{
	return textures[name];
}

SpriteSheet ResourceManager::LoadSpriteSheet(const char* file, bool alpha, std::string name, int texWidth, int texHeight)
{
    spriteSheets[name] = LoadSpriteSheetFromFile(file, alpha, texWidth, texHeight);
    return spriteSheets[name];
}
// Retrieves stored texture
SpriteSheet& ResourceManager::GetSpriteSheet(std::string name)
{
    //std::cout << spriteSheets[name].width << " / " << spriteSheets[name].textureWidth << " = " << static_cast<float>(spriteSheets[name].textureWidth) / spriteSheets[name].width << "\n";
    return spriteSheets[name];
}

// De-allocates all loaded resources
void ResourceManager::Clear()
{
	for (auto shader : shaders)
	{
		glDeleteProgram(shader.second.ID);
	}
	for (auto texture : textures)
	{
		glDeleteTextures(1, &texture.second.ID);
	}
}


// Loads and generates a shader from file
Shader ResourceManager::LoadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try
    {
        // open files
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        // close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        // if geometry shader path is present, also load a geometry shader
        if (gShaderFile != nullptr)
        {
            std::ifstream geometryShaderFile(gShaderFile);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::exception e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    const char* gShaderCode = geometryCode.c_str();
    // 2. now create shader object from source code
    Shader shader;
    shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
    return shader;
}

// Liads a single texture from file
Texture2D ResourceManager::LoadTextureFromFile(const char* file, bool alpha)
{
	// create texture object
	Texture2D texture;
    SDL_Surface* data = IMG_Load(file);
	if (alpha)
	{
		texture.internalFormat = GL_RGBA;
		texture.imageFormat = GL_RGBA;
        // there's a bug where SDL_Surface can't properly detect the pixel format of 8bit image
        // cast SDL_Surface to 32bit
        data = SDL_ConvertSurfaceFormat(data, SDL_PIXELFORMAT_RGBA32, 0);       //SDL_PIXELFORMAT_RBA8888 for 8bit themed
	}
	// load image
	
    /*
    if (data != nullptr)
    {
        std::cout << "Loaded texture from: " << file << std::endl;
    }*/
    
    
	// now generate texture
	texture.Generate(data);
	// and finally free image data
	SDL_FreeSurface(data);
	return texture;
}


SpriteSheet ResourceManager::LoadSpriteSheetFromFile(const char* file, bool alpha, int texWidth, int texHeight)
{
    // create texture object
    SpriteSheet spriteSheet(texWidth, texHeight);
    SDL_Surface* data = IMG_Load(file);
    if (alpha)
    {
        spriteSheet.internalFormat = GL_RGBA;
        spriteSheet.imageFormat = GL_RGBA;
        // there's a bug where SDL_Surface can't properly detect the pixel format of 8bit image
        // cast SDL_Surface to 32bit
        data = SDL_ConvertSurfaceFormat(data, SDL_PIXELFORMAT_RGBA32, 0);       //SDL_PIXELFORMAT_RBA8888 for 8bit themed
    }
    // load image

    /*
    if (data != nullptr)
    {
        std::cout << "Loaded texture from: " << file << std::endl;
    }*/


    // now generate texture
    spriteSheet.Generate(data);
    // and finally free image data
    SDL_FreeSurface(data);
    return spriteSheet;
}