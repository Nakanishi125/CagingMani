#pragma once

#include <cstdlib>
#include <fstream>

#include "Algorithm.h"
#include "GoalCondition.h"
#include "Configuration.h"


class RRT: public Algorithm
{
public:
    RRT(Node start);
    std::vector<Node> graph;

private:
    GoalCondition* gc;
    std::vector<Node> path;
    float range;
    std::ofstream file;

    bool Initialize();
    void Debug();
    void planning();
    void WriteToFile();
};


static std::string getDatetimeStr();