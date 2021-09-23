#pragma once

#include <cstddef>
#include <vector>

#include "square.h"
#include "maniMath.h"


class Wall
{
private:
	static Wall* instance;

	double horizontal;
	double vertical;
	Point2D origin;
	double direction;

	Wall()
		:horizontal(600),vertical(5.0),origin(0.0,-5.001),direction(0.0),
		 geometry()
	{
		getVertices();
	}

	void getVertices();

public:
	static Wall* getInstance();
	
	Square geometry;

};

