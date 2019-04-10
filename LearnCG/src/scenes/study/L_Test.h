#pragma once
#include "../Scene.h"

#include<iostream>
#include<string>
using namespace std;

class BillBoard : public Scene
{
public:
	BillBoard();
	~BillBoard();
	virtual void initGL();
	virtual void paintGL(float deltaTime);
	virtual void freeGL();


public:
	GLuint quadVAO;
	GLuint quadVBO;

	GLuint Texture;

};