#pragma once

#include "Shape.h"

class Cube : public Shape
{
public:
	Cube();
	~Cube();
	void draw(Shader* shader);
private:
	unsigned int VBO, VAO;
	static float CubeVertices[];
	string texture;
};

