#pragma once

#include <vector>
#include <math.h>
#include <cmath>
#include <cfloat>

class Point2D
{
public:
	Point2D(double _x, double _y) {
		x = _x;
		y = _y;
	}
	Point2D() {
		x = 0;
		y = 0;
	}
	Point2D(const Point2D& p){
		this->x = p.x;
		this->y = p.y;
	}
	Point2D& operator=(const Point2D& p){
		this->x=p.x;	this->y=p.y;	return *this;
	}

	double dist(const Point2D& p){
		return std::sqrt( (this->x-p.x)*(this->x-p.x) + (this->y-p.y)*(this->y-p.y) );
	}
	void zeroclear(){
		this->x = 0;	this->y = 0;
	}

	double x, y;
};


template<class T>
class State3D
{
public:
	State3D(T _x, T _y, T _theta) {
		x = _x;
		y = _y;
		th = _theta;
	}
	State3D() {
		x = 0;
		y = 0;
		th = 0;
	}
	// State3D(State3D s3d){
	// 	x = s3d.x;
	// 	y = s3d.y;
	// 	th = s3d.th;
	// }
	
	State3D& operator=(const State3D& s){
		this->x = s.x;	this->y = s.y;	this->th = s.th;	return *this;
	}

	bool operator==(const State3D &s){
		if(this->x  == s.x &&
			 this->y  == s.y &&
			 this->th == s.th){	return true;}
		else{ return false;}
	}
	
	T x;
	T y;
	T th;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   Vector Class  ///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Vector2D
{
public:
	Vector2D(double _x, double _y){
		x = _x;	y = _y;
	}

	Vector2D(){
		x = 0;  y= 0;
	}

	// to positional Vector
	Vector2D(const Point2D& p){
		x = p.x;	y = p.y;
	}

	Vector2D& operator/=(double d){
		this->x /= d;
		this->y /= d;
		return *this;
	}

	double norm(){
		return std::sqrt(x*x + y*y);
	}

	void normalize(){
		*this /= norm();
	}

	double dot(const Vector2D& vec)	const
	{
		return this->x*vec.x + this->y*vec.y;
	}

	double x;	
	double y;
};

class Vector3D
{
public:
	Vector3D(double _x, double _y, double _z){
		x = _x;	y = _y;	z = _z;
	}

	Vector3D(){
		x = 0;  y= 0;	z = 0;
	}

	Vector3D(const Vector2D& vec){
		x = vec.x;	y = vec.y;	z = 1;
	}

	Vector3D(const Point2D& pt){
		x = pt.x;	y = pt.y;	z = 1;
	}

	Vector3D& operator/=(double d){
		this->x /= d;
		this->y /= d;
		this->z /= d;
		return *this;
	}

	double norm(){
		return std::sqrt(x*x + y*y + z*z);
	}

	void normalize(){
		*this /= norm();
	}

	double dot(const Vector3D& vec)	const
	{
		return this->x*vec.x + this->y*vec.y + this->z*vec.z;
	}

	double x;	
	double y;
	double z;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   Matrix Class  ///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Mat22
{
public:
	Mat22(double e1, double e2, 
		  double e3, double e4)
		  {
			  p11 = e1;	p12 = e2;
			  p21 = e3; p22 = e4;
		  }
	
	Mat22()
	{
		p11 = 0; p12 = 0;
		p21 = 0; p22 = 0;
	}

	double p11, p12,
		  p21, p22;
};

class Mat33
{
public:
	Mat33(double e1, double e2, double e3,
		  double e4, double e5, double e6,
		  double e7, double e8, double e9)
	{
		p11 = e1;	p12 = e2; p13 = e3;
		p21 = e4; p22 = e5; p23 = e6;
		p31 = e7; p32 = e8; p33 = e9;
	}
	
	Mat33()
	{
		p11 = 0; p12 = 0; p13 = 0;
		p21 = 0; p22 = 0; p23 = 0;
		p31 = 0; p32 = 0; p33 = 0;
	}

	Mat33(const Mat22& mat)
	{
		p11 = mat.p11; p12 = mat.p12; p13 = 0;
		p21 = mat.p21; p22 = mat.p22; p23 = 0;
		p31 = 0; 	   p32 = 0; 	  p33 = 1;
	}

	Vector3D dot(const Vector3D& vec) const
	{
		double tx=0, ty=0, tz=0;
		tx = p11*vec.x + p12*vec.y + p13*vec.z;
		ty = p21*vec.x + p22*vec.y + p23*vec.z;
		tz = p31*vec.x + p32*vec.y + p33*vec.z;

		return Vector3D(tx,ty,tz);
	}
	
	double p11, p12, p13,
		  p21, p22, p23,
		  p31, p32, p33;
};

inline Vector2D operator*(Vector2D& v, Mat22& mat){
	return Vector2D(mat.p11*v.x+mat.p12*v.y, mat.p21*v.x+mat.p22*v.y);
}

inline Point2D operator+(Point2D& p, Vector2D& v){
	return Point2D(p.x+v.x, p.y+v.y);
}

double deg_to_rad(double deg);
double rad_to_deg(double rad);







