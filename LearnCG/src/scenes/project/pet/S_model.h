#pragma once

#include "../../Scene.h"
#include "LineFrameRenderer.h"

class S_model : public Scene
{
public:
	virtual void initGL();
	virtual void paintGL(float deltaTime);
	virtual void freeGL();
	virtual void initCamera();
	virtual void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
public:
	S_model();
	~S_model();
	LineFrameRenderer *lineFrameRenderer;
	Shader *shader;
	Model *petModel;
	Model *model20000;
	Model *model600000;
	bool showModel20000, showModel600000,showPetModel,showLineFrameRenderer;
};

