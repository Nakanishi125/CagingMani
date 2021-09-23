#pragma once

#include <cmath>

#include "Shape.h"


class Circle : public Shape
{
private:
	double R;	

public:
	Circle();
	Circle(double Radius);

	double getRadius();

	bool Intersection(Square poly);

};


