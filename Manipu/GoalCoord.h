#pragma once

#include "GoalCondition.h"
namespace bp = boost::property_tree;

class GoalCoord : public GoalCondition
{
public:
    State3D<int> dest;

    GoalCoord()
    :dest(), GoalCondition()
    {
        bp::ptree pt;
	    read_ini("../config/ProblemDefine.ini", pt);
        boost::optional<int> carrier = pt.get_optional<int>("goal.coordx");
        dest.x = carrier.get();
        carrier = pt.get_optional<int>("goal.coordy");
        dest.y = carrier.get();
        carrier = pt.get_optional<int>("goal.coordt");
        dest.th = carrier.get();
        // dest.x = goal.x;
        // dest.y = goal.y;
        // dest.th = goal.th;
    }

    bool judge(int index)
    {
        static double mini = DBL_MAX;
        Configuration* config = Configuration::getInstance();
        double kyori = config->toGoal(dest);
        if(mini > kyori){
			mini = kyori;
            min_index = index;
		}
		std::cout << "    Temporary minimum distance: ";	std::cout << mini << std::endl << std::endl;
        if(epsilon > mini){
            return true;
        }
        return false;
    }

};