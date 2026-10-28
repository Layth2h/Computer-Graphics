/////////////////////////////////////////////////////////////////////////////
//																									//
// Code framework for lecture computer graphics WS 2025/26 assignment 2 	//
//																									//
// A generic class for n-dimensional coordinate vectors							//
//																									//
// (c) Georg Umlauf																			//
//																									//
/////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Vector.h"
#include "Matrix.h"
#include "Color.h"

class Planet {
private:
	Color  color    = Red;
	int    radius   = 10;
	CVec2f position = CVec2f(0.0);

public:
	Planet() { }

	// getter
	Color  getColor   () const { return color; }
	int    getRadius  () const { return radius; }
	CVec2f getPosition() const { return position; }

	// setter
	void setColor   (const Color&  c) { color    = c; }
	void setRadius  (int           r) { radius   = std::max(1, r); }
	void setPosition(const CVec2f& p) { position = p; }

	// rotate point p by angle (in degrees) around the origin (affine coordinates)
	static CVec2f rotate(const CVec2f& p, float angle);

	// rotate point p by angle (in degrees) around the point c
	static CVec2f rotate(const CVec2f& p, const CVec2f& c, float angle);

	// homogeneous versions: 3x3 rotation matrices (angle in degrees)
	static CMat3f rotationMatrix(float angle);                    // around the origin
	static CMat3f rotationMatrix(const CVec2f& c, float angle);   // around the point c, as ONE matrix

	// draw
	void draw() const; // Implement in this method your Bresenham algorithm for circles

};