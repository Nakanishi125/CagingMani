#pragma once

#include <cmath>

#include "Shape.h"


class LShape : public Shape
{
private:
  	double long_side, short_side, long_pro, short_pro;
	static LShape* instance;
	std::vector<Point2D> vertices;

	LShape();

	void getVertices();

public:
	static Shape* getInstance();

	double getRadius();

	bool Intersection(Square poly);

};

