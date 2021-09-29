#include <chrono>

#include "Algorithm.h"
 
// Algorithm::Algorithm(Node start, State3D<int> goal, double epsilon)
// 	:Start(start), Goal(goal), epsilon(epsilon)
// {
// }

Algorithm::Algorithm(Node start)
	:Start(start)
{
}

void Algorithm::GeneratePath()
{
	planning();
}