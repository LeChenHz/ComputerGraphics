#pragma once

#include <glad/glad.h>
#include "stb_image.h"

#include <iostream>
#include <stdio.h>
#include "texture.h"

class Texture2D
{
public:
	Texture2D();
	void Generate(GLuint width, GLuint height, GLenum format, unsigned char *data);
	void Bind();
public:
	GLuint ID;
	GLuint Width, Height;
	GLuint Internal_Format;
	GLuint Image_Format;

	GLuint Wrap_S;
	GLuint Wrap_T;
	GLuint Filter_Min;
	GLuint Filter_Mag;
};