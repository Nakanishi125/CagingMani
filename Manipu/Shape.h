#pragma once

#include <vector>
#include <string>

#include "maniMath.h"
#include "square.h"
#include "triangle.h"

class Shape
{
public:	
	int symmetric_angle;

	static Shape* getInstance();
	void setCenterpoint(State3D<int> p);
	virtual double getRadius() = 0;
	virtual bool Intersect(Square other) = 0;

protected:
	Shape();
	State3D<int> center;

	virtual void setVertices() = 0;
};

