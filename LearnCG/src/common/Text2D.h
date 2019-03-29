#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Shader.h"
#include "texture.h"

#include <vector>

class Text2D
{
public:
	Text2D();
	~Text2D();
private:
	GLuint VAO;
	GLuint Text2DTextureID;
	GLuint Text2DVertexBufferID;
	GLuint Text2DUVBufferID;
	Shader *shader;
public:
	void printText2D(const char * text, int x, int y, int size);
};