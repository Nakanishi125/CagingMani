#include "Circle.h"

Circle::Circle()
	:R(10.0)
{
}

Circle::Circle(double Radius)
	:R(Radius)
{
}

double Circle::getRadius(){
	return R;
}

bool Circle::Intersection(square Poly)
{
	return true;
}
