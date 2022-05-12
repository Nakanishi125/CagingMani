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
	// std::vector<double> Linit = {40, -50, -30};
  	// std::vector<double> Rinit = {40, -50, -30};

	Node node(Linit,Rinit);

	Robot* robot = Robot::getInstance();
	robot->HandSetup(node);

	Algorithm* myAlgo = new RRT(node);

	// myAlgo->Debug();
	myAlgo->GeneratePath();
	myAlgo->WriteToFile();
	

	return 0;
}
