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

	std::vector<double> Linit = {-25, -15, -15};
  	std::vector<double> Rinit = {-80, 30, 80};
	// std::vector<double> Linit = {40, -50, -30};
  	// std::vector<double> Rinit = {40, -50, -30};

	Node node(Linit,Rinit);

	double epsilon = 20.0;

	Robot* robot = Robot::getInstance();
	robot->HandSetup(node);

	Algorithm* myAlgo = new RRT(node);

	myAlgo->GeneratePath();
	myAlgo->WriteToFile();
	

	return 0;
}
