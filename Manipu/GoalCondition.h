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
    GoalCondition()
    :epsilon()
    {
        bp::ptree pt;
	    read_ini("../config/ProblemdefConfig.ini", pt);
        boost::optional<double> carrier = pt.get_optional<double>("goal.epsilon");
	    epsilon = carrier.get();
    }
    virtual bool judge() = 0;

protected:
    double epsilon;
};