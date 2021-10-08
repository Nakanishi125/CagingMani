#include "Shape.h"
#include "LShape.h"
#include "Rectangle.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/optional.hpp>

namespace bp = boost::property_tree;
Shape::Shape()
	:center(0,0,0), symmetric_angle()
{
}


Shape* Shape::getInstance()
{
	Shape* instance = nullptr;
	bp::ptree pt;
	read_ini("../config/ObjectParameter.ini", pt);
	boost::optional<int> carrier = pt.get_optional<int>("target.shape");
	int sh = carrier.get();
	if(sh == 1){
		instance = Rectangle::getInstance();
	}
	else if(sh == 2){
		instance = LShape::getInstance();
	}

	return instance;
}


void Shape::setCenterpoint(State3D<int> p)
{
	center.x = p.x;
	center.y = p.y;
	center.th = p.th;

	setVertices();
}

