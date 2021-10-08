#pragma once

#include <vector>

#include "maniMath.h"

class OBB
{
public:
	double min;
	double max;

	OBB(std::vector<Point2D> poly, Vector2D dir)
		:min(DBL_MAX), max(-DBL_MAX)
	{
		for(int i=0; i<(int)poly.size(); ++i){
			Vector2D tmp(poly[i]);
			double dot = tmp.dot(dir);
			if(dot < min)	min = dot;
			if(dot > max)   max = dot;
		}
	}

	OBB()
		:min(), max()
	{}

	OBB(const OBB& obb)
		:min(), max()
	{
		min = obb.min;
		max = obb.max;
	}

};