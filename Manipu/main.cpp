#include <vector>

#include "maniMath.h"
#include "RRT.h"
#include "Robot.h"
#include "Wall.h"
#include "Shape.h"
#include "Node.h"

int main()
{
	Wall* wall = Wall::getInstance();
	Shape* obj = Shape::getInstance();

	std::vector<double> Linit = {-20, 20, -45};
  	std::vector<double> Rinit = {-60, 40, 50};

	Node node(Linit,Rinit);

	State3D<int> goal(-100, 220, 0);

	double epsilon = 45;

	Robot* robot = Robot::getInstance();
	robot->HandSetup(node);

	Algorithm* myAlgo = new RRT(node, goal, epsilon);

	myAlgo->GeneratePath();
	myAlgo->WriteToFile();
	

	return 0;
}
