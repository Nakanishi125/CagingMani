// #pragma once

// #include <array>
// #include <vector>

// #include "maniMath.h"


// class OBB
// {
// public:
// 	double min;
// 	double max;

// 	OBB(std::vector poly, Vector2D dir)
// 		:min(DBL_MAX), max(-DBL_MAX)
// 	{
// 		for(int i=0; i<poly.size(); ++i){
// 			Vector2D tmp(poly[i]);
// 			double dot = tmp.dot(dir);
// 			if(dot < min)	min = dot;
// 			if(dot > max) max = dot;
// 		}
// 	}
// };


// class Polygon
// {
// public:
// 	std::vector<Point2D> vertices;
		
// 	Polygon()
// 		:vertices()
// 	{
// 	}

// 	// Square(Point2D p1, Point2D p2, Point2D p3, Point2D p4)
// 	// 	:vertices()
// 	// {
// 	// 	vertices[0] = p1;
// 	// 	vertices[1] = p2;
// 	// 	vertices[2] = p3;
// 	// 	vertices[3] = p4;
// 	// }

// 	// intersect->false   not intersect->true
// 	bool intersect(Polygon other)
// 	{
//         int sz = this->vertices.size() + other.vertices.size();
// 		std::vector<Vector2D> axis(sz);
		
// 		// "this pointer's axis
//         int i=0;
//         for(; i<this->vertices.size()-1; i++){
//             Vector2D tmp(this->vertice[i+1].x - this->vertices[i].x, this->vertices[i+1].y - this->vertices[i].y);
//             axis.push_back(tmp);
//         }
//         Vector2D tmp(this->vertices[0].x - this->vertices[i].x, this->vertices[0].y - this->vertices[i].y);
//         axis.push_back(tmp);

//         for(i=0; i<this->other.vertices.size()-1; i++){
//             Vector2D tmp(other.vertices[i+1].x - other.vertices[i].x, other.vertices[i+1].y - other.vertices[i].y);
//             axis.push_back(tmp);
//         }
//         Vector2D tmp(other.vertices[0].x - other.vertices[i].x, other.vertices[0].y - other.vertices[i].y);
//         axis.push_back(tmp);

//         std::vector<OBB> A(sz);
// 		std::vector<OBB> B(sz);

//         for(int ind=0; ind<sz; ind++){
//         	OBB tmpA(this->vertices, axis[ind]);		
// 			OBB tmpB(other.vertices, axis[ind]);
// 			A[ind] = tmpA;
// 			B[ind] = tmpB;
//         }

//         for(int ind=0; ind<sz; ind++){
//             bool judge = A[ind].min > B[ind].max || A[ind].max < B[ind].min;
//             if(judge == true)   return true;
//         }

//         return false;

// 		// Vector2D tmp(this->vertices[1].x - this->vertices[0].x, 
// 		// 					   this->vertices[1].y - this->vertices[0].y);
// 		// tmp.normalize();
// 		// axis[0] = tmp;

// 		// tmp.x = this->vertices[2].x - this->vertices[1].x;
// 		// tmp.y = this->vertices[2].y - this->vertices[1].y;
// 		// tmp.normalize();
// 		// axis[1] = tmp;

// 		// // other square's axis
// 		// tmp.x = other.vertices[1].x - other.vertices[0].x;
// 		// tmp.y = other.vertices[1].y - other.vertices[0].y;
// 		// tmp.normalize();
// 		// axis[2] = tmp;

// 		// tmp.x = other.vertices[2].x - other.vertices[1].x;
// 		// tmp.y = other.vertices[2].y - other.vertices[1].y;
// 		// tmp.normalize();
// 		// axis[3] = tmp;

// 		//std::array<OBB, 4> A = {};
// 		//std::array<OBB, 4> B = {};
// 		// std::vector<OBB> A;
// 		// std::vector<OBB> B;

// 		// for(int ind=0; ind < axis.size(); ++ind)
// 		// {
// 		// 	OBB tmpA(this->vertices, axis[ind]);		
// 		// 	OBB tmpB(other.vertices, axis[ind]);
// 		// 	A.push_back(tmpA);
// 		// 	B.push_back(tmpB);
// 		// }

// 		// judge
// 		// bool judge = A[0].min > B[0].max ||
// 		// 			 A[0].max < B[0].min ||
// 		// 			 A[1].min > B[1].max ||
// 		// 			 A[1].max < B[1].min ||
// 		// 			 A[2].min > B[2].max ||
// 		// 			 A[2].max < B[2].min ||
// 		// 			 A[3].min > B[3].max ||
// 		// 			 A[3].max < B[3].min ;
// 		// // true -> Not intersect , false -> intersect

// 		// return judge;
// 	}
	
// };



