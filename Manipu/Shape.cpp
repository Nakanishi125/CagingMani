#include "Shape.h"
#include "LShape.h"
#include "Rectangle.h"

Shape::Shape()
	:center(0,0,0), symmetric_angle()
{
}


Shape* Shape::getInstance()
{
	// else ~ 
	
	Shape* instance = Rectangle::getInstance();

	return instance;
}


void Shape::setCenterpoint(State3D<int> p)
{
	center.x = p.x;
	center.y = p.y;
	center.th = p.th;
}

bool Shape::Intersection(Square poly)
{
	
	return false;
}

