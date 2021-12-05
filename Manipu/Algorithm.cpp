#include <chrono>

#include "Algorithm.h"


Algorithm::Algorithm(Node start)
	:Start(start)
{
}

void Algorithm::GeneratePath()
{
	planning();
}