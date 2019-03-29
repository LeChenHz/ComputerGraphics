#include "Texture2D.h"

Texture2D::Texture2D()
{
	this->Width = 0;
	this->Height = 0;
	this->Internal_Format = GL_RGB;
	this->Image_Format = GL_RGB;
	this->Wrap_S = GL_REPEAT;
	this->Wrap_T = GL_REPEAT;
	//this->Filter_Min = GL_LINEAR_MIPMAP_LINEAR;
	this->Filter_Min = GL_LINEAR;
	this->Filter_Mag = GL_LINEAR;

	glGenTextures(1, &this->ID);
}

void Texture2D::Generate(GLuint width, GLuint height, GLenum format, unsigned char * data)
{
	this->Width = width;
	this->Height = height;
	this->Internal_Format = format;
	this->Image_Format = format;
	glBindTexture(GL_TEXTURE_2D, this->ID);
	glTexImage2D(GL_TEXTURE_2D, 0, this->Internal_Format, width, height, 0, this->Image_Format, GL_UNSIGNED_BYTE, data);
	//glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Mag);

	// ½â°óÎÆÀí
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Bind()
{
	glBindTexture(GL_TEXTURE_2D, this->ID);
}
