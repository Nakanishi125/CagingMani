#pragma once

#include <cmath>

#include "Shape.h"


class Rectangle : public Shape
{
private:
  	double short_side, long_side;
	static Rectangle* instance;
	Square geometry;

	Rectangle();

	void setVertices();

public:
	static Shape* getInstance();

	bool Intersect(Square poly);
	double getRadius();
};



			
