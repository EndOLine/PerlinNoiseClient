#pragma once
/*
* Program: clsCartCoord
* Description: Handle Cartesian Coordinates
*			   This is to replace clsMesh (the old one)
* Log:
*	20221130: created
*   20221202: new operator* operator/ with floats
*/
#ifndef CLSCARTCOORD_H
#define CLSCARTCOORD_H
#define PI 3.14159f
//const double PI = 3.141592653589793238462643383279502884197;
//const double PI = 3.14159265358979;
#define RADIANS(degrees) ((degrees) * (PI / 180.0f))
#include <cmath>

class clsCartCoord {
	//parameters
public:	
	float x = 0, y = 0, z = 0;
private:

	//functions
public:
	clsCartCoord();
	clsCartCoord(const float X, const float Y, const float Z);
	float Length();
	float DistanceTo(const clsCartCoord To);
	void  Normalize();							// change to a Unit vector
	clsCartCoord GetUnitVector();				// return a unit vector but don't change original
	float DotProduct(const clsCartCoord Vec2);
	void CrossProduct(const clsCartCoord Vec1, const clsCartCoord Vec2);
	void Subtract(const clsCartCoord v1, const clsCartCoord v2);
	void Add(const clsCartCoord v1, const clsCartCoord v2);
	void Multiply(const clsCartCoord v1, const float k);
	void Divide(const clsCartCoord v1, const float k);
	void ComputeNormal(const clsCartCoord v1, const clsCartCoord v2, const clsCartCoord v3);

	// assignment operator
	clsCartCoord& operator=(const clsCartCoord& rhs);
	// compound assignment operators
	clsCartCoord& operator+=(const clsCartCoord& rhs);
	clsCartCoord& operator-=(const clsCartCoord& rhs);
	clsCartCoord& operator*=(const float& rhs);
	clsCartCoord& operator/=(const float& rhs);

	// Binary Arithmetic Operators
	const clsCartCoord operator+(const clsCartCoord& other) const;
	const clsCartCoord operator-(const clsCartCoord& other) const;
	const clsCartCoord operator*(const float& rhs) const;
	const clsCartCoord operator/(const float& rhs) const;

	//Comparison Operators
	bool operator==(const clsCartCoord& other) const;
	bool EqualTo(const clsCartCoord& A, const float epsilon = 0.0005f) const;
	bool operator!=(const clsCartCoord& other) const;
private:

};


#endif