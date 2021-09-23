#pragma once
#include "Algorithm.h"
#include "Configuration.h"


class RRT: public Algorithm
{
public:
    RRT(Node start, State3D<int> goal, double epsilon);

private:
    std::vector<Node> graph;
    std::vector<Node> path;
    float range;

    void planning();
    void WriteToFile();
};

static std::string getDatetimeStr();