#include "ResourceManager.h"

ResourceManager* ResourceManager::instance = nullptr;

ResourceManager * ResourceManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new ResourceManager();
	}
	return instance;
}

Texture2D ResourceManager::LoadTexture(const GLchar * file, string name)
{
	Textures[name] = loadTextureFromFile(file);
	return Textures[name];
}

Texture2D ResourceManager::GetTexture(string name)
{
	return Textures[name];
}

void ResourceManager::Clear()
{
	for (auto iter : Textures)
		glDeleteTextures(1, &iter.second.ID);
}

Texture2D ResourceManager::loadTextureFromFile(const GLchar * path)
{
	Texture2D texture;
	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		texture.Generate(width, height, format, data);

		stbi_image_free(data);
	}
	else
	{
		cout << "Texture failed to load at path: " << path << endl;
		stbi_image_free(data);
	}
	return texture;
}
