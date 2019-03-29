#pragma once

#include "../Scene.h"

class S_fog : public Scene
{
public:
	virtual void initGL();
	virtual void paintGL(float deltaTime);
	virtual void freeGL();
public:
	S_fog();
	~S_fog();
private:
	std::vector<glm::vec3> objectPositions;
	Shader *shader;
	Model *ourModel;
};

