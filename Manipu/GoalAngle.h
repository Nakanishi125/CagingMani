#pragma once

#include "GoalCondition.h"
namespace bp = boost::property_tree;
extern Node nownode;

class GoalAngle : public GoalCondition
{
public:
    GoalAngle()
    :dest(), GoalCondition()
    {
        bp::ptree pt;
	    read_ini("../config/ProblemdefConfig.ini", pt);
        boost::optional<double> carrier = pt.get_optional<double>("goal.ang1");
        dest.push_back(carrier.get());
        carrier = pt.get_optional<double>("goal.ang2");
        dest.push_back(carrier.get());
        carrier = pt.get_optional<double>("goal.ang3");
        dest.push_back(carrier.get());
        carrier = pt.get_optional<double>("goal.ang4");
        dest.push_back(carrier.get());
        carrier = pt.get_optional<double>("goal.ang5");
        dest.push_back(carrier.get());
        carrier = pt.get_optional<double>("goal.ang6");
        dest.push_back(carrier.get());
    }

    bool judge()
    {
        static double mini = DBL_MAX;
        Configuration* config = Configuration::getInstance();
        double kyori = config->toGoal(nownode, dest);
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
    std::vector<double> dest;
};