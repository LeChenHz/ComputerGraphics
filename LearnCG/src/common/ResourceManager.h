#pragma once

#include <glad/glad.h>

#include "Texture2D.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <string>

using namespace std;

class ResourceManager 
{
private:
	static ResourceManager * instance;
public:
	static ResourceManager * GetInstance();
	map<string, Texture2D> Textures;
	
	Texture2D LoadTexture(const GLchar *file, string name);
	Texture2D GetTexture(string name);

	void Clear();
private:
	ResourceManager() {};
	Texture2D loadTextureFromFile(const GLchar *path);
};
