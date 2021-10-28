#pragma once

#include <cmath>
#include <array>

#include "Shape.h"


class LShape : public Shape
{
private:
  	double long_side, short_side, long_pro, short_pro;
	static LShape* instance;
	std::array<Square, 2> geometry;
	std::vector<Point2D> vertices;

	LShape();
	void setVertices();

public:
	static Shape* getInstance();

	double getRadius();

	bool Intersect(Square poly);

};

