#pragma once
#include <vector>
#include <string>
#include <fstream>

#include "maniMath.h"
#include "square.h"


class Link
{
public:
	const int height;
	const int width;

	double AbsAngle;
	Point2D bottom_center;
	Point2D next_center;
	Mat22 coord;
	Square geometry;

	Link(int h, int w, double ang, Point2D center, Mat22 mat)
		:height(h), width(w), AbsAngle(ang), bottom_center(center), 
		 next_center(), coord(mat), geometry()
	{
		Update();
	}
	
	void Update()
	{
		double radAngle = deg_to_rad(AbsAngle);
		Vector2D trs;
		
		trs.x = -0.5*width*std::cos(radAngle);		trs.y = -0.5*width*std::sin(radAngle);
		trs = trs*coord;
		Point2D refPoint = bottom_center + trs;

		trs.x = width*std::cos(radAngle);	trs.y = width*std::sin(radAngle);
		trs = trs*coord;
		Point2D p2 = refPoint + trs;

		trs.x = -height*std::sin(radAngle);	trs.y = height*std::cos(radAngle);
		trs = trs*coord;
		Point2D p3 = p2 + trs;

		trs.x = -width*std::cos(radAngle);	trs.y = -width*std::sin(radAngle);
		trs = trs*coord;
		Point2D p4 = p3 + trs;

		Vector2D cen_trs(-height*std::sin(radAngle), height*std::cos(radAngle));
		cen_trs = cen_trs*coord;

		next_center = bottom_center + cen_trs;



		geometry.setVertices(refPoint, p2, p3, p4);
	}

	double getRadius(){
		return std::sqrt( (height*height) + (width*width) )/2 ;
	}

};
