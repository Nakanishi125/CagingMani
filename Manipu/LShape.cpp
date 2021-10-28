
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
	double radAngle = deg_to_rad(center.th);
	double aspect = atan2(long_side, short_side);

	Point2D refPoint(center.x - 0.5*std::sqrt(short_side*short_side + long_side*long_side)*std::cos(aspect + radAngle),
					 center.y - 0.5*std::sqrt(short_side*short_side + long_side*long_side)*std::sin(aspect + radAngle));
	Point2D p2(refPoint.x + short_side*std::cos(radAngle), refPoint.y + short_side*std::sin(radAngle));
	Point2D p3(p2.x - long_pro*std::sin(radAngle), p2.y + long_pro*std::cos(radAngle));
	Point2D p4(p3.x - short_side*std::cos(radAngle), p3.y - short_side*std::sin(radAngle));	//no check

	geometry[0].setVertices(refPoint, p2, p3, p4);

	Point2D p1(p4);
	p2.x = p1.x + short_pro*std::cos(radAngle);
	p2.y = p1.y + short_pro*std::sin(radAngle);
	p3.x = p2.x - (long_side - long_pro)*std::sin(radAngle);
	p3.y = p2.y + (long_side - long_pro)*std::cos(radAngle);
	p4.x = p3.x - short_pro*std::cos(radAngle);
	p4.y = p3.y - short_pro*std::sin(radAngle);

	geometry[1].setVertices(p1, p2, p3, p4);
}

bool LShape::Intersect(Square poly)
{
	bool judge = geometry[0].intersect(poly) || geometry[1].intersect(poly);
	return judge;
}

