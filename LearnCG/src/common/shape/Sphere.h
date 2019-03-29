#pragma once

#include "Shape.h"
#include <vector>
using namespace std;

class Sphere : public Shape
{
public:
	Sphere();
	~Sphere();
	void draw(Shader* shader);
	void renderSphere(unsigned int, unsigned int);
private:
	Shader* shader;

	static float vertex[];
	static float UpVertex[];
	static float MiddleVertex[];
	static float DownVertex[];

	unsigned int VAO, VBO;

	int xSegments, ySegments;//用于判断顶点要求是否发生变化
	string diffuseMap, normalMap;
};

