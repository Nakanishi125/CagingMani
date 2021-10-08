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

void Wall::setVertices()
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

	geometry.setVertices(p1, p2, p3, p4);
		
}
