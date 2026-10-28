/////////////////////////////////////////////////////////////////////////////
//																									//
// Code framework for lecture computer graphics WS 2025/26 assignment 2  	//
//																									//
// (c) Georg Umlauf																			//
//																									//
/////////////////////////////////////////////////////////////////////////////

#include "Planet.h"
#include "glut.h"
#include <cmath>

//
// Implement HERE your Bresenham algorithm for circles
//
// draws the 8 symmetric points of the circle (must be called between glBegin/glEnd)
static void plot8(int xm, int ym, int x, int y)
{
	glVertex2i(xm + x, ym + y);
	glVertex2i(xm - x, ym + y);
	glVertex2i(xm + x, ym - y);
	glVertex2i(xm - x, ym - y);
	glVertex2i(xm + y, ym + x);
	glVertex2i(xm - y, ym + x);
	glVertex2i(xm + y, ym - x);
	glVertex2i(xm - y, ym - x);
}

CVec2f Planet::rotate(const CVec2f& p, float angle)
{
	float a = angle * 3.14159265f / 180.0f;		// degrees -> radians
	float c = cos(a);
	float s = sin(a);

	// x' = x*cos - y*sin,  y' = x*sin + y*cos
	return CVec2f(c * p[0] - s * p[1],
	              s * p[0] + c * p[1]);
}

// rotating around c = move c to the origin, rotate there, move back
CVec2f Planet::rotate(const CVec2f& p, const CVec2f& c, float angle)
{
	CVec2f moved = CVec2f(p[0] - c[0], p[1] - c[1]);
	CVec2f turned = rotate(moved, angle);
	return CVec2f(turned[0] + c[0], turned[1] + c[1]);
}

//     | cos -sin 0 |
// R = | sin  cos 0 |
//     |  0    0   1 |
CMat3f Planet::rotationMatrix(float angle)
{
	float a = angle * 3.14159265f / 180.0f;		// degrees -> radians
	float c = cos(a);
	float s = sin(a);

	CMat3f R = CMat3f::Identity();
	R(0, 0) = c;	R(0, 1) = -s;
	R(1, 0) = s;	R(1, 1) =  c;
	return R;
}

// T(c) * R * T(-c) written out as one matrix:
//     | R   c - R*c |
//     | 0       1   |
// i.e. the rotation part R stays, only the translation column changes
CMat3f Planet::rotationMatrix(const CVec2f& c, float angle)
{
	CMat3f M = Planet::rotationMatrix(angle);

	M(0, 2) = c[0] - (M(0, 0) * c[0] + M(0, 1) * c[1]);
	M(1, 2) = c[1] - (M(1, 0) * c[0] + M(1, 1) * c[1]);
	return M;
}

void Planet::draw() const
{
	int xm = int(position[0]);
	int ym = int(position[1]);

	glBegin(GL_POINTS);
	glColor3d(color.r, color.g, color.b);

	// mid-point
	glVertex2i(xm, ym);

	int x = 0;
	int y = radius;
	int d = 5 - 4 * radius;

	plot8(xm, ym, x, y);

	while (y > x)
	{
		if (d >= 0)
		{
			d += 4 * (2 * (x - y) + 5);	// SE
			x++;
			y--;
		}
		else
		{
			d += 4 * (2 * x + 3);		// E
			x++;
		}
		plot8(xm, ym, x, y);
	}

	glEnd();
}