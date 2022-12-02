/*
* Program: clsCartCoord
* Description: Handle Cartesian Coordinates
* notes: 
*   for operator overloading see:
*	http://courses.cms.caltech.edu/cs11/material/cpp/donnie/cpp-ops.html
* Log:
*	20221130: created
*
*/
#include "clsCartCoord.h"

clsCartCoord::clsCartCoord() {
	x = 0.0f, y = 0.0f, z = 0.0f; 
};
clsCartCoord::clsCartCoord(const float X, const float Y, const float Z) { x = X, y = Y, z = Z; };
float clsCartCoord::Length() { 
	return sqrtf((x * x) + (y * y) + (z * z)); 
};
float clsCartCoord::DistanceTo(const clsCartCoord To) {
	clsCartCoord Hold;
	//Hold.Subtract({ x,y,z }, To);
	Hold.Subtract(*this, To);
	return Hold.Length();
}
void  clsCartCoord::Normalize() { 
	float l = Length(); 
	x /= l; y /= l; z /= l; 
};							
clsCartCoord clsCartCoord::GetUnitVector() { 
	float l = Length(); 
	return { x / l,y / l,z / l }; 
};		

// show how similar the vectors are
// 0.0f is not similar.  
// 1.0f is very similar. 
// < 0.0f point in oppsite direction
//> 0.0f pointing in same direction
// normally the dot product is (V1 x V2) / |V2|. since v2 is a unit vector it will be equal to one.
float clsCartCoord::DotProduct(const clsCartCoord Vec2) {							// return DOT product of local and parameter; with unit vectors, this projects one line on the other.
	return (x * Vec2.x) + (y * Vec2.y) + (z * Vec2.z);								// asume length of V2 is 1.0f;
};

void clsCartCoord::CrossProduct(const clsCartCoord Vec1, const clsCartCoord Vec2) {	// returns line perpendicular to the other two lines.  (multiplication?)
	x = (Vec1.y * Vec2.z) - (Vec1.z * Vec2.y);										// change local to the cross product of the two input vectors
	y = (Vec1.z * Vec2.x) - (Vec1.x * Vec2.z);
	z = (Vec1.x * Vec2.y) - (Vec1.y * Vec2.x);
};
void clsCartCoord::Subtract(const clsCartCoord v1, const clsCartCoord v2) {
	x = v1.x - v2.x;
	y = v1.y - v2.y;
	z = v1.z - v2.z;
};
void clsCartCoord::Add(const clsCartCoord v1, const clsCartCoord v2) {
	x = v1.x + v2.x;
	y = v1.y + v2.y;
	z = v1.z + v2.z;
};
void clsCartCoord::Multiply(const clsCartCoord v1, const float k) {
	x = v1.x * k;
	y = v1.y * k;
	z = v1.z * k;
};
void clsCartCoord::Divide(const clsCartCoord v1, const float k) {
	x = v1.x / k;
	y = v1.y / k;
	z = v1.z / k;
};
void clsCartCoord::ComputeNormal(const clsCartCoord v1, const clsCartCoord v2, const clsCartCoord v3) {
	clsCartCoord A, B, C;
	A.Subtract(v1, v2);
	B.Subtract(v2, v3);
	C.CrossProduct(A, B);
	if (C.Length() > 0.0f) {
		C.Normalize();
	};
	x = C.x; y = C.y; z = C.z;
};

clsCartCoord& clsCartCoord::operator=(const clsCartCoord& rhs) {
	// Only do assignment if RHS is a different object from this.
	if (this != &rhs) {
	 // Deallocate, allocate new space, copy values...
		x = rhs.x; y = rhs.y; z = rhs.z;
	};

	return *this;
}

clsCartCoord& clsCartCoord::operator+=(const clsCartCoord& rhs) {
	// watch out for the possibility that rhs may be this object. This depends on the definition of this class
	// Do the compound assignment work.
	x += rhs.x; y += rhs.y; z += rhs.z;
	return *this;
}

clsCartCoord& clsCartCoord::operator-=(const clsCartCoord& rhs) {
	// watch out for the possibility that rhs may be this object. This depends on the definition of this class
	// Do the compound assignment work.
	x -= rhs.x; y -= rhs.y; z -= rhs.z;
	return *this;
}

clsCartCoord& clsCartCoord::operator*=(const float& rhs) {
	x *= rhs; y *= rhs; z *= rhs;
	return *this;
}

clsCartCoord& clsCartCoord::operator/=(const float& rhs) {
	x /= rhs; y /= rhs; z /= rhs;
	return *this;
}

const clsCartCoord clsCartCoord::operator+(const clsCartCoord& other) const {
	clsCartCoord result = *this;	// Make a copy of myself.  Same as MyClass result(*this);
	result += other;				// Use += to add other to the copy.
	return result;					// All done!
}

const clsCartCoord clsCartCoord::operator-(const clsCartCoord& other) const {
	clsCartCoord result = *this;	// Make a copy of myself.  Same as MyClass result(*this);
	result -= other;				// Use -= to add other to the copy.
	return result;					// All done!
}

const clsCartCoord clsCartCoord::operator*(const float& rhs) const {
	clsCartCoord result = *this;
	result *= rhs;
	return result;
}

const clsCartCoord clsCartCoord::operator/(const float& rhs) const {
	clsCartCoord result = *this;
	result /= rhs;
	return result;
}

// compare floats by comparing to an epsilon value
bool clsCartCoord::EqualTo(const clsCartCoord& A, const float epsilon) const {
	if ((fabs(x - A.x) < epsilon) && (fabs(y - A.y) < epsilon) && (fabs(z - A.z) < epsilon)) {
		return true;
	};
	return false;
}

bool clsCartCoord::operator==(const clsCartCoord& other) const {
	// Compare the values, and return a bool result.
	bool bResult = EqualTo(other);
	return (EqualTo(other));
}
bool clsCartCoord::operator!=(const clsCartCoord& other) const {
	return !(*this == other);
}