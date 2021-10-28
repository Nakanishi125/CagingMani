#pragma once

#include <cmath>

#include "Shape.h"


class Triangulus : public Shape
{
private:
  	double e1, e2, e3;
	static Triangulus* instance;
	Triangle geometry;

	Triangulus();

	void setVertices();

public:
	double R;
	static Shape* getInstance();

	bool Intersect(Square poly);
	double getRadius();
};


