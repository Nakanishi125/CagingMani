#include <iostream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/optional.hpp>

#include "GoalCoord.h"
#include "GoalAngle.h"

class ProblemFactory
{
public:
    static GoalCondition* create()
    {
        GoalCondition* instance = nullptr;
        bp::ptree pt;
        read_ini("../config/ProblemdefConfig.ini", pt);
        boost::optional<int> carrier = pt.get_optional<int>("goal.mode");
        int mode = carrier.get();
        if(mode == 1){
            instance = new GoalCoord();
        }
        else if(mode == 2){
            instance = new GoalAngle();
        }

        return instance;
    }
};