#pragma once

#include "GoalCondition.h"
namespace bp = boost::property_tree;

class GoalCoord : public GoalCondition
{
public:
    GoalCoord()
    :dest(), GoalCondition()
    {
        bp::ptree pt;
	    read_ini("../config/ProblemdefConfig.ini", pt);
        boost::optional<int> carrier = pt.get_optional<int>("goal.coordx");
        dest.x = carrier.get();
        carrier = pt.get_optional<int>("goal.coordy");
        dest.y = carrier.get();
        carrier = pt.get_optional<int>("goal.coordt");
        dest.th = carrier.get();
    }

    bool judge()
    {
        static double mini = DBL_MAX;
        Configuration* config = Configuration::getInstance();
        double kyori = config->toGoal(dest);
        if(mini > kyori){
			mini = kyori;
		}
		std::cout << "Now, minimum distance is ";	std::cout << mini << std::endl << std::endl;
        if(epsilon > kyori){
            return true;
        }
        return false;
    }


private:
    State3D<int> dest;
};