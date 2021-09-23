#pragma once
#include "Algorithm.h"
#include "Configuration.h"


class RRTPSO: public Algorithm
{
public:
    RRT(Node start, State3D<int> goal, double epsilon);

private:
// RRT系統
    std::vector<Node> graph;
    std::vector<Node> path;
    float range;

    void planning();
    void WriteToFile();

// PSO系統
    int number_of_particles;
    int dim;
    int repeat_times;

    void update_positions();
    void update_velocities();
    void PSO();
};
