#pragma once

#include <vector>

#include "maniMath.h"
#include "OBB.h"


class Square
{
public:
	std::vector<Point2D> vertices;
		
	Square()
		:vertices()
	{
		vertices.resize(4);
	}

	Square(Point2D p1, Point2D p2, Point2D p3, Point2D p4)
		:vertices()
	{
		vertices.resize(4);
		vertices[0] = p1;
		vertices[1] = p2;
		vertices[2] = p3;
		vertices[3] = p4;
	}

	Square(std::vector<Point2D> v)
		:vertices()
	{
		vertices = 	v;
	}

	Square(const Square& sq)
		:vertices()
	{
		this->vertices = sq.vertices;
	}

	void setVertices(const Point2D& p1, const Point2D& p2, 		
					 const Point2D& p3, const Point2D& p4)
	{
		vertices[0] = p1;
		vertices[1] = p2;
		vertices[2] = p3;
		vertices[3] = p4;
	}

	bool intersect(Square other)
	{
		std::vector<Vector2D> axis(4);
		
		// "this pointer's axis
		Vector2D tmp(this->vertices[1].x - this->vertices[0].x, 
							   this->vertices[1].y - this->vertices[0].y);
		tmp.normalize();
		axis[0] = tmp;

		tmp.x = this->vertices[2].x - this->vertices[1].x;
		tmp.y = this->vertices[2].y - this->vertices[1].y;
		tmp.normalize();
		axis[1] = tmp;

		// other square's axis
		tmp.x = other.vertices[1].x - other.vertices[0].x;
		tmp.y = other.vertices[1].y - other.vertices[0].y;
		tmp.normalize();
		axis[2] = tmp;

		tmp.x = other.vertices[2].x - other.vertices[1].x;
		tmp.y = other.vertices[2].y - other.vertices[1].y;
		tmp.normalize();
		axis[3] = tmp;


		std::vector<OBB> A(4);
		std::vector<OBB> B(4);

		for(int ind=0; ind < (int)axis.size(); ++ind)
		{
			OBB tmpA(this->vertices, axis[ind]);		
			OBB tmpB(other.vertices, axis[ind]);
			A[ind] = tmpA;
			B[ind] = tmpB;
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
	}
	
};



