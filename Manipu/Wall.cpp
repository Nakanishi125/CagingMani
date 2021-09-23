#include "Wall.h"

Wall* Wall::instance = NULL;

Wall* Wall::getInstance()
{
	if(instance == NULL)
	{
		instance = new Wall();
	}
	return instance;
}

void Wall::getVertices()
{
	double rad = deg_to_rad(direction);


	Point2D p1(origin.x - 0.5 * horizontal * std::cos(rad),
						 origin.y - 0.5 * horizontal * std::sin(rad) );
	Point2D p2(p1.x - vertical * std::sin(rad),
						 p1.y + vertical * std::cos(rad) );
	Point2D p3(p2.x + horizontal * std::cos(rad),
						 p2.y + horizontal * std::sin(rad) );
	Point2D p4(p3.x + vertical * std::sin(rad),
						 p3.y - vertical * std::cos(rad) );

	geometry.vertices[0] = p1;		
	geometry.vertices[1] = p2;		
	geometry.vertices[2] = p3;		
	geometry.vertices[3] = p4;		
}
