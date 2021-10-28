#pragma once

#include <vector>

#include "maniMath.h"
#include "OBB.h"


class Triangle
{
public:
	std::vector<Point2D> vertices;
		
	Triangle()
		:vertices()
	{
		vertices.resize(3);
	}

	Triangle(Point2D p1, Point2D p2, Point2D p3)
		:vertices()
	{
		vertices.resize(3);
		vertices[0] = p1;
		vertices[1] = p2;
		vertices[2] = p3;
	}

	Triangle(const Triangle& tri)
		:vertices()
	{
		this->vertices = tri.vertices;
	}

	void setVertices(const Point2D& p1, const Point2D& p2, const Point2D& p3)
	{
		vertices[0] = p1;
		vertices[1] = p2;
		vertices[2] = p3;
	}

	//     intersect -> false
	// Not intersect -> true
	bool intersect(Square other)
	{
		std::vector<Vector2D> axis(5);
		
		// "this pointer's axis
		Vector2D tmp(this->vertices[1].x - this->vertices[0].x, 
							   this->vertices[1].y - this->vertices[0].y);
		tmp.normalize();
		axis[0] = tmp;

		tmp.x = this->vertices[2].x - this->vertices[1].x;
		tmp.y = this->vertices[2].y - this->vertices[1].y;
		tmp.normalize();
		axis[1] = tmp;

        tmp.x = this->vertices[0].x - this->vertices[2].x;
		tmp.y = this->vertices[0].y - this->vertices[2].y;
		tmp.normalize();
		axis[2] = tmp;

		// other square's axis
		tmp.x = other.vertices[1].x - other.vertices[0].x;
		tmp.y = other.vertices[1].y - other.vertices[0].y;
		tmp.normalize();
		axis[3] = tmp;

		tmp.x = other.vertices[2].x - other.vertices[1].x;
		tmp.y = other.vertices[2].y - other.vertices[1].y;
		tmp.normalize();
		axis[4] = tmp;


		std::vector<OBB> A;
		std::vector<OBB> B;

		for(int ind=0; ind < (int)axis.size(); ++ind)
		{
			OBB tmpA(this->vertices, axis[ind]);		
			OBB tmpB(other.vertices, axis[ind]);
			// A[ind] = tmpA;
			// B[ind] = tmpB;
			A.push_back(tmpA);
			B.push_back(tmpB);
		}

		// judge
		// Not intersect -> true	intersect -> false
		// but for return value, true -> intersect, false -> NOT intersect
		bool judge = false;
		for(int ind=0; ind < (int)axis.size(); ++ind){
			judge = A[ind].min > B[ind].max || A[ind].max < B[ind].min;
			if(judge == true)
				return false;
		}

		return true;

		// bool judge = A[0].min > B[0].max ||
		// 						 A[0].max < B[0].min ||
		// 						 A[1].min > B[1].max ||
		// 						 A[1].max < B[1].min ||
		// 						 A[2].min > B[2].max ||
		// 						 A[2].max < B[2].min ||
		// 						 A[3].min > B[3].max ||
		// 						 A[3].max < B[3].min ;
		// // true -> Not intersect , false -> intersect

		// return judge;
	}
	
};



