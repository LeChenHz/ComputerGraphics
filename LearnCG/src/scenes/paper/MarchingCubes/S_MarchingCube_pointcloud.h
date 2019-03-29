#pragma once

#include "../../Scene.h"

class S_MarchingCube_pointcloud : public Scene
{
public:
	virtual void initGL();
	virtual void paintGL(float deltaTime);
	virtual void freeGL();
};

