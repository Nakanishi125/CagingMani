#include <cmath>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/optional.hpp>

#include <string>
#include <fstream>

#include "Robot.h"

namespace bp = boost::property_tree;

Robot* Robot::getInstance()
{
	if(instance == NULL)
		{
			instance = new Robot();
		}
	return instance;
}


void Robot::HandSetup(Node& node)
{
	bp::ptree pt;
	read_ini("../config/RobotConfig.ini", pt);

// Hand size setup	
	std::vector<int> Lheight(4);
	std::vector<int> Lwidth(4);
	std::vector<int> Rheight(4);
	std::vector<int> Rwidth(4);

	boost::optional<int> carrier = pt.get_optional<int>("size.Lh1");
	Lheight[0] = carrier.get();
	carrier = pt.get_optional<int>("size.Lh2");
	Lheight[1] = carrier.get();
	carrier = pt.get_optional<int>("size.Lh3");
	Lheight[2] = carrier.get();
	carrier = pt.get_optional<int>("size.Lh4");
	Lheight[3] = carrier.get();

	carrier = pt.get_optional<int>("size.Lw1");
	Lwidth[0] = carrier.get();
	carrier = pt.get_optional<int>("size.Lw2");
	Lwidth[1] = carrier.get();
	carrier = pt.get_optional<int>("size.Lw3");
	Lwidth[2] = carrier.get();
	carrier = pt.get_optional<int>("size.Lw4");
	Lwidth[3] = carrier.get();

	carrier = pt.get_optional<int>("size.Rh1");
	Rheight[0] = carrier.get();
	carrier = pt.get_optional<int>("size.Rh2");
	Rheight[1] = carrier.get();
	carrier = pt.get_optional<int>("size.Rh3");
	Rheight[2] = carrier.get();
	carrier = pt.get_optional<int>("size.Rh4");
	Rheight[3] = carrier.get();

	carrier = pt.get_optional<int>("size.Rw1");
	Rwidth[0] = carrier.get();
	carrier = pt.get_optional<int>("size.Rw2");
	Rwidth[1] = carrier.get();
	carrier = pt.get_optional<int>("size.Rw3");
	Rwidth[2] = carrier.get();
	carrier = pt.get_optional<int>("size.Rw4");
	Rwidth[3] = carrier.get();

// Hand origin setup
	boost::optional<float> carrier2 = pt.get_optional<float>("origin.lx");
	Lorg.x = carrier2.get();
	carrier2 = pt.get_optional<float>("origin.ly");
	Lorg.y = carrier2.get();

	carrier2 = pt.get_optional<float>("origin.rx");
	Rorg.x = carrier2.get();
	carrier2 = pt.get_optional<float>("origin.ry");
	Rorg.y = carrier2.get();

// Hand coordinate setup
// 		--        --
// 		|  00  10  |
//		|  01  11  |
// 		--        --

	float p00 = 0.0, p01 = 0.0, p10 = 0.0, p11 = 0.0;
	carrier = pt.get_optional<float>("coordinate.l00");
	p00 = carrier.get();
	carrier = pt.get_optional<float>("coordinate.l01");
	p01 = carrier.get();
	carrier = pt.get_optional<float>("coordinate.l10");
	p10 = carrier.get();
	carrier = pt.get_optional<float>("coordinate.l11");
	p11 = carrier.get();
	Mat22 lmat(p00, p01, p10, p11);

	carrier = pt.get_optional<float>("coordinate.r00");
	p00 = carrier.get();
	carrier = pt.get_optional<float>("coordinate.r01");
	p01 = carrier.get();
	carrier = pt.get_optional<float>("coordinate.r10");
	p10 = carrier.get();
	carrier = pt.get_optional<float>("coordinate.r11");
	p11 = carrier.get();
	Mat22 rmat(p00, p01, p10, p11);


	// left
	// Root Link generating
	double AbsAngle = 0.0;
	Point2D center(Lorg);
	Link link0L(Lheight[0], Lwidth[0], AbsAngle, center, lmat);
	Lhand.push_back(link0L);

	// each Link generating
	for(int l=0; l<node.Ldof; l++)
	{
		AbsAngle += node.node[l];
		double radAngle = deg_to_rad(AbsAngle);
		Link link(Lheight[l+1], Lwidth[l+1] , AbsAngle, Lhand.back().next_center, lmat);
		Lhand.push_back(link);
	}

	// right	
	// Root Link generating
	center = Rorg;
	AbsAngle = 0.0;
	Link link0R(Rheight[0], Rwidth[0], AbsAngle, center, rmat);
	Rhand.push_back(link0R);

	// each Link generating
	for(int r=0; r<node.Rdof; r++)
	{
		AbsAngle += node.node[r+node.Ldof];
		double radAngle = deg_to_rad(AbsAngle);
		Link link(Rheight[r+1], Rwidth[r+1] , AbsAngle, Rhand.back().next_center, rmat);
		Rhand.push_back(link);
	}

}


void Robot::Update(Node& node)
{
// Left hand
	double AbsAngle = 0.0;
	Lhand[0].AbsAngle = AbsAngle;
	// Root Link Update

	Lhand[0].Update();
	// Each Link Update

	for(int i=0; i<Node::Ldof; i++){
		AbsAngle += node.node[i];
		Lhand[i+1].AbsAngle = AbsAngle;
		Lhand[i+1].bottom_center = Lhand[i].next_center;
		Lhand[i+1].Update();
	}

// Right hand
	AbsAngle = 0.0;
	Rhand[0].AbsAngle = AbsAngle;

	// Root Link Update
	Rhand[0].Update();

	// Each Link Update
	for(int i=0; i<Node::Rdof; i++){
		AbsAngle += node.node[node.Ldof+i];
		Rhand[i+1].AbsAngle = AbsAngle;
		Rhand[i+1].bottom_center = Rhand[i].next_center;
		Rhand[i+1].Update();
	}


}

bool Robot::rrIntersect()
{
	for(int i=0; i<Lhand.size(); ++i){
		for(int j=0; j<Rhand.size(); ++j){
			if(Lhand[i].geometry.intersect(Rhand[j].geometry))	return true;
		}
	}
	
	return false;
}


bool Robot::rwIntersect(Wall* wall)
{
	for(int i=0; i<Lhand.size(); ++i){
		if( Lhand[i].geometry.intersect(wall->geometry) )	return true;
	}

	for(int i=0; i<Rhand.size(); ++i){
		if( Rhand[i].geometry.intersect(wall->geometry) ) return true;
	}

	return false;
}

Robot* Robot::instance = NULL;


