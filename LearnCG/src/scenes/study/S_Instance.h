#pragma once

#include "../Scene.h"

class S_Instance : public Scene
{
public:
	virtual void initGL();
	virtual void paintGL(float deltaTime);
	virtual void freeGL();
public:
	S_Instance();
	~S_Instance();
	Shader *planetShader, *asteroidShader;
	Model *rock;
	Model *planet;
	unsigned int amount; //ÑÒÊ¯ÊýÁ¿
	glm::mat4* modelMatrices;

	unsigned int VBO;
};

