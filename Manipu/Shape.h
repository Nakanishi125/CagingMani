#pragma once

#include <vector>
#include <string>

#include "maniMath.h"
#include "square.h"

class Shape
{
public:	
	int symmetric_angle;

	virtual double getRadius() = 0;

	virtual bool Intersection(Square poly);

	static Shape* getInstance();

	void setCenterpoint(State3D<int> p);

protected:
	Shape();

	Square geometry;
	State3D<int> center;
};

