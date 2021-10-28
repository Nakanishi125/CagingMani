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

	std::vector<double> Linit = {72, -25, -61};
  	std::vector<double> Rinit = {19, 13, 45};

	Node node(Linit,Rinit);
	//std::vector<double> goal = {37, -22, -39, 21, -31, -27};
	// State3D<int> goal(-100, 220, 0);

	double epsilon = 5.0;

	Robot* robot = Robot::getInstance();
	robot->HandSetup(node);

	Algorithm* myAlgo = new RRT(node);

	myAlgo->GeneratePath();
	myAlgo->WriteToFile();
	

	return 0;
}
