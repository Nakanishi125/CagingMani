#pragma once
#include "Node.h"


class Algorithm
{
protected:
	double epsilon;
    Node Start;
	State3D<int> Goal;

	Algorithm(Node start, State3D<int> goal, double epsilon);

private:
	virtual void planning() = 0;

public:
	void GeneratePath();
	virtual void WriteToFile() = 0;
};

