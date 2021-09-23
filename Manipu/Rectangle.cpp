#include <cmath>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/optional.hpp>

#include "Rectangle.h"

namespace bp = boost::property_tree;

Rectangle::Rectangle()
	:Shape(), long_side(0), short_side(0)
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

void Rectangle::getVertices()
{
	double radAngle = deg_to_rad(center.th);
	double aspect = atan2(short_side, long_side);

// 違くね？ 2021/09/15
	// Point2D refPoint(center.x - 0.5*std::sqrt(short_side*short_side + long_side*long_side)*std::sin(radAngle + aspect),
	// 								 center.y - 0.5*std::sqrt(short_side*short_side + long_side*long_side)*std::cos(radAngle + aspect) );
	// Point2D p2(refPoint.x + short_side*std::cos(radAngle), refPoint.y - short_side*std::sin(radAngle));
	// Point2D p3(p2.x + long_side*std::sin(radAngle), p2.y + long_side*std::cos(radAngle));
	// Point2D p4(p3.x - short_side*std::cos(radAngle), p3.y + short_side*std::sin(radAngle));

//　修正 これも違うくね？ 2021/09/17
	// Point2D refPoint(center.x - 0.5*std::sqrt(short_side*short_side + long_side*long_side)*std::cos(radAngle + aspect),
	// 				 center.y - 0.5*std::sqrt(short_side*short_side + long_side*long_side)*std::sin(radAngle + aspect));
	// Point2D p2(refPoint.x + short_side*std::cos(radAngle), refPoint.y + short_side*std::sin(radAngle));
	// Point2D p3(p2.x - long_side*std::sin(radAngle), p2.y + long_side*std::cos(radAngle));
	// Point2D p4(p3.x - short_side*std::cos(radAngle), p3.y - short_side*std::sin(radAngle));

	Point2D refPoint(center.x - 0.5*std::sqrt(short_side*short_side + long_side*long_side)*std::sin(aspect - radAngle),
					 center.y - 0.5*std::sqrt(short_side*short_side + long_side*long_side)*std::cos(aspect - radAngle));
	Point2D p2(refPoint.x + short_side*std::cos(radAngle), refPoint.y + short_side*std::sin(radAngle));
	Point2D p3(p2.x - long_side*std::sin(radAngle), p2.y + long_side*std::cos(radAngle));
	Point2D p4(p3.x - short_side*std::cos(radAngle), p3.y - short_side*std::sin(radAngle));

	geometry.vertices[0] = refPoint;
	geometry.vertices[1] = p2;
	geometry.vertices[2] = p3;
	geometry.vertices[3] = p4;
}


bool Rectangle::Intersection(Square poly)
{
	getVertices();

	// Axis Vector extracted
	// Index means axis No.
	std::vector<Vector2D> axis;
	Vector2D tmp(poly.vertices[1].x-poly.vertices[0].x, poly.vertices[1].y-poly.vertices[0].y);
	tmp.normalize();
	axis.push_back(tmp);

	tmp.x = poly.vertices[2].x-poly.vertices[1].x;
	tmp.y = poly.vertices[2].y-poly.vertices[1].y;
	tmp.normalize();
	axis.push_back(tmp);

	tmp.x = geometry.vertices[1].x - geometry.vertices[0].x;
	tmp.y = geometry.vertices[1].y - geometry.vertices[0].y;
	tmp.normalize();
	axis.push_back(tmp);

	tmp.x = geometry.vertices[2].x - geometry.vertices[1].x;
	tmp.y = geometry.vertices[2].y - geometry.vertices[1].y;
	tmp.normalize();
	axis.push_back(tmp);

	// search min, max along with each axis
	std::vector<OBB> A;
	std::vector<OBB> B;

	for(int ind=0; ind < axis.size(); ++ind)
	{
		OBB tmpA(poly.vertices, axis[ind]);		
		OBB tmpB(geometry.vertices, axis[ind]);

		A.push_back(tmpA);
		B.push_back(tmpB);
	}

	// judge
	bool judge= A[0].min > B[0].max ||
				A[0].max < B[0].min ||
				A[1].min > B[1].max ||
				A[1].max < B[1].min ||
				A[2].min > B[2].max ||
				A[2].max < B[2].min ||
				A[3].min > B[3].max ||
				A[3].max < B[3].min ;
	// true -> Not intersect , false -> Intersect

	return judge;
}


