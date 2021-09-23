
#include "LShape.h"

// #include <boost/property_tree/ptree.hpp>
// #include <boost/property_tree/ini_parser.hpp>
// #include <boost/optional.hpp>

// namespace bp = boost::property_tree;

LShape::LShape()
	:long_side(0), short_side(0), long_pro(0), short_pro(0)
{
	// bp::ptree pt;
	// read_ini("../config/ObjectParameter.ini", pt);

	// boost::optional<int> carrier = pt.get_optional<int>("LShape.long_side");
	// long_side = carrier.get();
	// carrier = pt.get_optional<int>("LShape.short_side");
	// short_side = carrier.get();
	// carrier = pt.get_optional<int>("LShape.long_pro");
	// long_pro = carrier.get();
	// carrier = pt.get_optional<int>("LShape.short_pro");
	// short_pro = carrier.get();
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

void LShape::getVertices()
{
	// State3D<int> center is a center of LShape's circumscribe circle.
//	vertices.clear();

//	double radAngle = deg_to_rad(center.th);
//	Point2D refPoint(center.x - std::sqrt(short_side*short_side + long_side*long_side)*std::cos(radAngle),
//									 center.y - std::sqrt(short_side*short_side + long_side*long_side)*std::sin(radAngle) );
//	vertices.push_back(refPoint);
//
//	Point2D p2(refPoint.x + short_side*std::cos(radAngle), refPoint.y + short_side*std::sin(radAngle));
//	vertices.push_back(p2);
//
//	Point2D p3(p2.x - long_side*std::sin(radAngle), p2.y + long_side*std::cos(radAngle));
//	vertices.push_back(p3);
//
//	Point2D p4(p3.x - short_side*std::cos(radAngle), p3.y - short_side*std::sin(radAngle));
//	vertices.push_back(p4);

}


bool LShape::Intersection(Square poly)
{

	return true;
}

