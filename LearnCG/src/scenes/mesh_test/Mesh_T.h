#pragma once
#include "../Scene.h"

#include<iostream>
#include<string>
using namespace std;

class BillBoard : public Scene
{
public:
	virtual void initGL();
	virtual void paintGL(float deltaTime);
	virtual void freeGL();
	BillBoard();
	~BillBoard();

public:
	GLuint quadVAO;
	GLuint quadVBO;
	GLfloat quadVertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
		 -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f
	};

	GLuint Texture;

};