#pragma once

#include "../Shader.h"
#include "../ResourceManager.h"

class Shape {
public:
	virtual void draw(Shader* shader) = 0;
};