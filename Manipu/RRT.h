#pragma once
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
    void confirm(int signum, RRT* rrt);
    void Initialize();
    void planning();
    void WriteToFile();
};


static std::string getDatetimeStr();