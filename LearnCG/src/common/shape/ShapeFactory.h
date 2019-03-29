#pragma once

#include "Shape.h"
#include "Cube.h"
#include "Sphere.h"
#include <string>
using namespace std;

enum ShapeType {
	Cube_Type,
	Sphere_Type
};

class ShapeFactory
{
public:
	ShapeFactory();
	Shape* getShape(ShapeType shapeType);
};

