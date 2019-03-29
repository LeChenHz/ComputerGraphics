#include "ShapeFactory.h"



ShapeFactory::ShapeFactory()
{

}

Shape* ShapeFactory::getShape(ShapeType shapeType)
{
	if (shapeType == Cube_Type)
	{
		return new Cube();
	}
	else if (shapeType == Sphere_Type)
	{
		return new Sphere();
	}
	return nullptr;
}
