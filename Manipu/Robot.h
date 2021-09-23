#pragma once

#include <vector>

#include "Node.h"
#include "Algorithm.h"
#include "Link.h"
#include "maniMath.h"
#include "Wall.h"

class Robot
{
public:
	// Left hand and Right hand member respectively
	std::vector<Link> Lhand;
	std::vector<Link> Rhand;

	// The origin of both hands
	Point2D Lorg;
	Point2D Rorg;
	int lmode;
	int rmode;
		
	static Robot* getInstance();

	void HandSetup(Node& node);
	void Update(Node& node);

	bool rrIntersect();
	bool rwIntersect(Wall* wall);


private:
	// Constructor : must not be called external function
	Robot(){}

	static Robot* instance;

	void RefSetting(std::vector<Link> &hand, Point2D org);

};

