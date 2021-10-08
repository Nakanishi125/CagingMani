#include <cmath>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/optional.hpp>

#include "Rectangle.h"

namespace bp = boost::property_tree;

Rectangle::Rectangle()
	:Shape(), long_side(0), short_side(0), geometry()
{
	symmetric_angle = 180;
	bp::ptree pt;
	read_ini("../config/ObjectParameter.ini", pt);

	boost::optional<int> carrier = pt.get_optional<int>("Rectangle.long_side");
	long_side = carrier.get();
	carrier = pt.get_optional<int>("Rectangle.short_side");
	short_side = carrier.get();
}

Rectangle* Rectangle::instance = nullptr;

Shape* Rectangle::getInstance()
{
		if(instance == nullptr) 
		{
			instance = new Rectangle();
		}
		return instance;
}

double Rectangle::getRadius()
{
	return sqrt(short_side*short_side + long_side*long_side );
}

void Rectangle::setVertices()
{
	double radAngle = deg_to_rad(center.th);
	double aspect = atan2(short_side, long_side);

	Point2D refPoint(center.x - 0.5*std::sqrt(short_side*short_side + long_side*long_side)*std::sin(aspect - radAngle),
					 center.y - 0.5*std::sqrt(short_side*short_side + long_side*long_side)*std::cos(aspect - radAngle));
	Point2D p2(refPoint.x + short_side*std::cos(radAngle), refPoint.y + short_side*std::sin(radAngle));
	Point2D p3(p2.x - long_side*std::sin(radAngle), p2.y + long_side*std::cos(radAngle));
	Point2D p4(p3.x - short_side*std::cos(radAngle), p3.y - short_side*std::sin(radAngle));

	geometry.setVertices(refPoint, p2, p3, p4);

}


bool Rectangle::Intersect(Square poly)
{
	bool judge = geometry.intersect(poly);
	return judge;
}


