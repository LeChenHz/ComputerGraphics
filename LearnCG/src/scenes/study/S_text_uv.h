#pragma once

#include "../Scene.h"
#include "../../common/Text2D.h"

class S_text_uv : public Scene
{
public:
	virtual void initGL();
	virtual void paintGL(float deltaTime);
	virtual void freeGL();
public:
	S_text_uv();
	~S_text_uv();
	Text2D *text2D;
};

