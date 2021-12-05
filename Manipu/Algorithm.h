#pragma once
#include "Node.h"

class Algorithm
{
protected:
    Node Start;

	Algorithm(Node start);

private:
	virtual void planning() = 0;

public:
	void GeneratePath();
	virtual void WriteToFile() = 0;
};