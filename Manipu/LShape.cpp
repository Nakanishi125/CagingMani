
#include "LShape.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/optional.hpp>

namespace bp = boost::property_tree;

LShape::LShape()
	:long_side(0), short_side(0), long_pro(0), short_pro(0)
{
	bp::ptree pt;
	read_ini("../config/ObjectParameter.ini", pt);

	boost::optional<int> carrier = pt.get_optional<int>("LShape.long_side");
	long_side = carrier.get();
	carrier = pt.get_optional<int>("LShape.short_side");
	short_side = carrier.get();
	carrier = pt.get_optional<int>("LShape.long_pro");
	long_pro = carrier.get();
	carrier = pt.get_optional<int>("LShape.short_pro");
	short_pro = carrier.get();

	vertices.resize(6);
}

LShape* LShape::instance = NULL;

Shape* LShape::getInstance()
{
		if(instance == NULL)
		{
			instance = new LShape();
		}
		return instance;
}

double LShape::getRadius()
{
	return sqrt(long_side * long_side + short_side * short_side);
}

void LShape::setVertices()
{
	// State3D<int> center is a center of LShape's circumscribe circle.

}

bool LShape::Intersect(Square poly)
{

	return true;
}

