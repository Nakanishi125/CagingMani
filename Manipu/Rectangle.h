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

	void getVertices();

public:
	static Shape* getInstance();

	double getRadius();

	bool Intersection(Square poly);

};



			
