#pragma once

#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/optional.hpp>

#include "maniMath.h"
#include "Configuration.h"
namespace bp = boost::property_tree;

class GoalCondition
{
public:
    double epsilon;
    State3D<int> goal;

    GoalCondition()
    :epsilon()
    {
        bp::ptree pt;
	    read_ini("../config/ProblemDefine.ini", pt);
        boost::optional<double> carrier = pt.get_optional<double>("goal.epsilon");
	    epsilon = carrier.get();
        carrier = pt.get_optional<int>("goal.coordx");
        goal.x = carrier.get();
        carrier = pt.get_optional<int>("goal.coordy");
        goal.y = carrier.get();
        carrier = pt.get_optional<int>("goal.coordt");
        goal.th = carrier.get();
    }
    virtual bool judge(int index) = 0;

protected:
    int min_index;      // for path generating when interrupting program
};