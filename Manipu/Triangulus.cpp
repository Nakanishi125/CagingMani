#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/optional.hpp>

#include "Triangulus.h"

namespace bp = boost::property_tree;

Triangulus* Triangulus::instance = nullptr;

Triangulus::Triangulus()
    :e1(), e2(), e3(), geometry(), R(), symmetric_angle(360)
{
	bp::ptree pt;
	read_ini("../config/ObjectParameter.ini", pt);

    boost::optional<int> carrier = pt.get_optional<int>("Triangle.edge1");
	e1 = carrier.get();
    carrier = pt.get_optional<int>("Triangle.edge2");
    e2 = carrier.get();
    carrier = pt.get_optional<int>("Triangle.edge3");
    e3 = carrier.get();

    double under = 2*sqrt(1 - ( (e2*e2+e3*e3-e1*e1)/(2*e2*e3)  * (e2*e2+e3*e3-e1*e1)/(2*e2*e3) ) );
    this->R = e1/under;
}

Shape* Triangulus::getInstance()
{
    if(instance == nullptr)
    {
        instance = new Triangulus();
    }
    return instance;
}

double Triangulus::getRadius()
{
    return this->R;
}

void Triangulus::setVertices()
{
    double radAngle = deg_to_rad(center.th);
    double alpha = acos(e1/(2*this->R));
    double beta  = acos(e3/(2*this->R));

    Point2D refPoint(center.x - 0.5*this->R*cos(alpha + radAngle),
                     center.y - 0.5*this->R*sin(alpha + radAngle));
    Point2D p2(refPoint.x + e1*cos(radAngle), refPoint.y + e1*sin(radAngle));
    Point2D p3(refPoint.x + e3*cos(radAngle + alpha + beta), refPoint.y + e3*sin(radAngle + alpha + beta));

    geometry.setVertices(refPoint, p2, p3);
}

bool Triangulus::Intersect(Square poly)
{  
    bool judge = geometry.intersect(poly);
    return judge;
}