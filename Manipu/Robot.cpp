#include <cmath>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/optional.hpp>

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
	Point2D ref(center.x - 0.5*Lwidth[0] , center.y);
	Link link0L(Lheight[0], Lwidth[0], AbsAngle, ref, lmat);
	Lhand.push_back(link0L);
	center.y += Lheight[0];

	// each Link generating
	for(int l=0; l < node.Ldof; l++)
	{
		AbsAngle += node.node[l];
		double radAngle = deg_to_rad(AbsAngle);
		Point2D ref(center.x - 0.5*Lwidth[l+1]*cos(radAngle) , center.y - 0.5*Lwidth[l+1]*sin(radAngle));
		Link link(Lheight[l+1], Lwidth[l+1] , AbsAngle, ref, lmat);
		Lhand.push_back(link);
		center.x -= Lheight[l+1]*std::sin(radAngle);
		center.y += Lheight[l+1]*std::cos(radAngle);
	}

	// right	
	// Root Link generating
	AbsAngle = 0.0;
	center=Rorg;
	Point2D Rref(center.x - 0.5*Rwidth[0] , center.y);
	ref = Rref;
	Link link0R(Rheight[0], Rwidth[0], AbsAngle, ref, rmat);
	Rhand.push_back(link0R);
	center.y += Rheight[0];

	// each Link generating
	for(int r=0; r<node.Rdof; r++)
	{
		AbsAngle += node.node[r+node.Ldof];
		double radAngle = deg_to_rad(AbsAngle);
		Point2D ref(center.x - 0.5*Rwidth[r+1]*std::cos(radAngle) , center.y - 0.5*Rwidth[r+1]*std::sin(radAngle) );
		Link link(Rheight[r+1], Rwidth[r+1] , AbsAngle, ref, rmat);
		Rhand.push_back(link);
		center.x -= Rheight[r+1]*std::sin(radAngle);
		center.y += Rheight[r+1]*std::cos(radAngle);
	}

}

void Robot::RefSetting(std::vector<Link> &hand, Point2D org)
{
	double AbsAngle = 0.0;
	Point2D center(org);
	for(int i=0; i<hand.size(); i++){
		double radAngle = deg_to_rad(hand[i].AbsAngle);
		Point2D ref(center.x - 0.5*hand[i].width*std::cos(radAngle), center.y - 0.5*hand[i].width*std::sin(radAngle));
		hand[i].refPoint = ref;

		center.x -= hand[i].height*std::sin(radAngle);
		center.y += hand[i].height*std::cos(radAngle);
	}
}


void Robot::Update(Node& node)
{
	// Left hand
	double AbsAngle = 0.0;
	Lhand[0].AbsAngle = AbsAngle; 
	for(int i=0; i<Node::Ldof; i++){
		AbsAngle += node.node[i];
		Lhand[i+1].AbsAngle = AbsAngle;
	}
	RefSetting(Lhand, Lorg);

	for(int i=0; i<=node.Ldof; i++)
		Lhand[i].Update();

	// Right hand
	AbsAngle = 0.0;
	Rhand[0].AbsAngle = AbsAngle;
	for(int i=0; i<node.Rdof; i++){
		AbsAngle += node.node[node.Ldof+i];
		Rhand[i+1].AbsAngle = AbsAngle;
	}
	RefSetting(Rhand, Rorg);

	for(int i=0; i<=node.Rdof; i++)
		Rhand[i].Update();

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


