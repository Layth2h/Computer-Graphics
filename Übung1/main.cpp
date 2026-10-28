/////////////////////////////////////////////////////////////////////////////
//																									//
// Code framework for lecture computer graphics WS 2025/26 assignment 1 	//
//																									//
// A generic class for n-dimensional coordinate vectors							//
//																									//
// (c) Georg Umlauf																			//
//																									//
/////////////////////////////////////////////////////////////////////////////

// Include-Ffile for io
#include <iostream>
using namespace std;

#include "Point.h"
#include "Color.h"

// Include-File for GLUT-Library
#include "glut.h"

//////////////////////////////////////////////////////////////////////////
//
// some initializations
//
//

// resolution of the complete texture
// !!!ATTENTION!!! some resolutions for the texture do not work -> POT2 problem
#define TEX_RES_X 60
#define TEX_RES_Y 60
// number of pixels of the texture
#define TEX_RES TEX_RES_X *TEX_RES_Y
// length of axes for the texture (axes are asymmetric from -TexRes#/2 to TesRes#/2-1)
#define TEX_HALF_X TEX_RES_X / 2
#define TEX_HALF_Y TEX_RES_Y / 2
// converting indices from (x,y) coordinats to a linear array used in the texture buffer
#define TO_LINEAR(x, y) (((x)) + TEX_RES_X * ((y)))

// global memory for texture
char g_Buffer[3 * TEX_RES];
// texture ID name
GLuint g_TexID = 0;

// resolution of the of the main window (can be changed by the user)
int g_WinWidth = 800;
int g_WinHeight = 800;

// Function to organize the texture.
void manageTexture()
{
	glEnable(GL_TEXTURE_2D);
	if (g_TexID == 0)
		glGenTextures(1, &g_TexID);
	glBindTexture(GL_TEXTURE_2D, g_TexID);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TEX_RES_X, TEX_RES_Y, 0, GL_RGB, GL_UNSIGNED_BYTE, g_Buffer);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

// Callback Function to change the window size.
void reshape(int w, int h)
{
	g_WinWidth = w;
	g_WinHeight = h;
	glViewport(0, 0, w, h); // Establish viewport to cover entire window.

	glMatrixMode(GL_PROJECTION);				 // Start modifying the projection matrix.
	glLoadIdentity();							 // Reset project matrix.
	glOrtho(-w / 2, w / 2, -h / 2, h / 2, 0, 1); // Map abstract coords directly to window coords.

	glutPostRedisplay();
}

/////////////////////////////////////////////////////////////////////////////////////////
//
// Here starts the part relevant for the exercise.
//

//
// Function clears the viewport/display with the given color
// - Usage z.B.: clearImage (Color (1,1,1)) clears tht viewport with white
// - If no parameter is given, white is used as default.
//
void clearDisplay(Color c = Color())
{
	for (int i = 0; i < TEX_RES; i++)
	{
		g_Buffer[3 * i] = int(255.0 * c.r);
		g_Buffer[3 * i + 1] = int(255.0 * c.g);
		g_Buffer[3 * i + 2] = int(255.0 * c.b);
	}
}

//
// Functions plot a point at the given coordinates p in color c
// - Usage z.B.: setPoint (Point(10, 5), Color (1,0,0)) plots red point at (10, 5)
// - It no color is given, black is used as default
//	- USE THIS FUNCTION TO PLOT POINT IN YOUR BESENHAM
//
void setPoint(Point p, Color c = Color(0, 0, 0))
{
	int x = p.x + TEX_HALF_X;
	int y = p.y + TEX_HALF_Y;
	if (x < 0 || y < 0 || x >= TEX_RES_X || y >= TEX_RES_Y)
	{
		cerr << "Illegal point co-ordinates (" << p.x << ", " << p.y << ")\n"
			 << flush;
		return;
	}

	g_Buffer[3 * TO_LINEAR(x, y)] = int(255.0 * c.r);
	g_Buffer[3 * TO_LINEAR(x, y) + 1] = int(255.0 * c.g);
	g_Buffer[3 * TO_LINEAR(x, y) + 2] = int(255.0 * c.b);
}

//
// Assignment 1 Exercise 1
//
// Implement this function to draw a line from p1 to p2 in color c using Besenham's algorithm.
// - Use setPoint to plot individual points/pixels.
//
// void BresenhamLine(Point p1, Point p2, Color c)
// {
// 	// first point
// 	setPoint(Point(p1), c);

// 	// ...
// 	int x, y, dx, dy, d, dNE, dE;
// 	x = p1.x;
// 	y = p1.y;
// 	dx = p2.x - x;
// 	dy = p2.y - y;
// 	dNE = 2 * (dy - dx);
// 	dE = 2 * dy;
// 	d = 2 * dy - dx;

// 	while (x < p2.x)
// 	{
// 		if (d >= 0) // NE
// 		{
// 			d += dNE;
// 			x++;
// 			y++;
// 		}
// 		else // E
// 		{
// 			d += dE;
// 			x++;
// 		}
// 		setPoint(Point(x, y), c);
// 	}

// 	// ...
// 	// last point
// 	setPoint(Point(p2), c);
// }

void BresenhamLineFirstOctant(Point p1, Point p2, Color c)
{
	int dx = p2.x - p1.x;
	int dy = p2.y - p1.y;

	int D = 2 * dy - dx;
	int x = p1.x;
	int y = p1.y;

	setPoint(Point(x, y), c);

	while (x < p2.x)
	{
		x++;
		if (D > 0)
		{
			y++;
			D += (2 * (dy - dx));
		}
		else
		{
			D += 2 * dy;
		}
		setPoint(Point(x, y), c);
	}
}

void BresenhamLineLow(Point p1, Point p2, Color c)
{
	int dx = p2.x - p1.x;
	int dy = p2.y - p1.y;

	int yi = 1;

	if (dy < 0)
	{
		yi = -1;
		dy = -dy;
	}

	int D = 2 * dy - dx;
	int x = p1.x;
	int y = p1.y;

	setPoint(Point(x, y), c);

	while (x < p2.x)
	{
		x++;
		if (D > 0)
		{
			y += yi;
			D += (2 * (dy - dx));
		}
		else
		{
			D += 2 * dy;
		}
		setPoint(Point(x, y), c);
	}
}

void BresenhamLineHigh(Point p1, Point p2, Color c)
{
	int dx = p2.x - p1.x;
	int dy = p2.y - p1.y;

	int xi = 1;

	if (dx < 0)
	{
		xi = -1;
		dx = -dx;
	}

	int D = (2 * dx) - dy;
	int x = p1.x;
	int y = p1.y;

	setPoint(Point(x, y), c);

	while (y < p2.y)
	{
		y++;
		if (D > 0)
		{
			x += xi;
			D += (2 * (dx - dy));
		}
		else
		{
			D += 2 * dx;
		}
		setPoint(Point(x, y), c);
	}
}

void plotLine(Point p1, Point p2, Color c)
{
	if (abs(p2.y - p1.y) < abs(p2.x - p1.x))
	{
		if (p1.x > p2.x)
			BresenhamLineLow(p2, p1, c);
		else
			BresenhamLineLow(p1, p2, c);
	}
	else
	{
		if (p1.y > p2.y)
			BresenhamLineHigh(p2, p1, c);
		else
			BresenhamLineHigh(p1, p2, c);
	}
}

// Assignment 1 Exercise 1
// part 2
void BresenhamLine2(Point p1, Point p2, Color c)
{
	// first point
	setPoint(Point(p1), c);
	;

	// ...
	int x, y, dx, dy, sx, sy, d, dNE, dE;

	dx = abs(p2.x - p1.x);
	dy = abs(p2.y - p1.y);
	sx = (p2.x > p1.x) ? 1 : -1; // step direction in x
	sy = (p2.y > p1.y) ? 1 : -1; // step direction in y
	bool steep = dy > dx;

	if (steep)
	{
		int t;
		t = dx;
		dx = dy;
		dy = t;
	}

	x = p1.x;
	y = p1.y;

	dNE = 2 * (dy - dx);
	dE = 2 * dy;
	d = 2 * dy - dx;

	for (int i = 0; i < dx; i++)
	{
		if (d >= 0) // NE
		{
			d += dNE;
			x += sx;
			y += sy;
		}
		else // E
		{
			d += dE;
			if (steep)
				y += sy;
			else
				x += sx;
		}
		setPoint(Point(x, y), c);
	}

	// ...
	// last point
	setPoint(Point(p2), c);
}

// ...
// implemented by my to make the plotting easier
void plot8(int xm, int ym, int x, int y, Color c)
{
	setPoint(Point(xm + x, ym + y), c);
	setPoint(Point(xm - x, ym + y), c);
	setPoint(Point(xm + x, ym - y), c);
	setPoint(Point(xm - x, ym - y), c);
	setPoint(Point(xm + y, ym + x), c);
	setPoint(Point(xm - y, ym + x), c);
	setPoint(Point(xm + y, ym - x), c);
	setPoint(Point(xm - y, ym - x), c);
}

// ...

//
// Assignment 1 Exercise 2
// Implement this function to draw a circle with radius r around mid-point p in color c using Besenham's algorithm.
// - Use setPoint to plot individual points/pixels.
// - Also plot the mid-point.
//
void BresenhamCircle(Point p, int r, Color c)
{
	// mid-point
	setPoint(Point(p), c);

	// ...
	int x, y, d, dSE, dE;
	x = 0;
	y = r;
	d = 5 - 4 * r;

	plot8(p.x, p.y, x, y, c);

	while (y > x)
	{
		if (d >= 0)
		{
			dSE = 4 * (2 * (x - y) + 5);
			d += dSE;
			x++;
			y--;
		}
		else
		{
			dE = 4 * (2 * x + 3);
			d += dE;
			x++;
		}
		plot8(p.x, p.y, x, y, Color(c.r + 0.3f, c.g + 0.2f, c.b - 0.3f));
	}

	// ...
}

//
// This (callback) function triggers a redraw of the viewport/display.
// - Please consider only the part between the comments.
//
void display(void)
{
	Color background(0.5, 0.5, 0.5); // grey background
	clearDisplay(background);		 // clears the old display

	//////////////////////////////////////////////////////////////////
	//
	// Here the functions to draw lines and circles are invoked.
	// - Define here the relevant parameters.
	// - Call   here the relevant functions.
	// - Try    here different parameter settings, i.e. start point, end point, mid-point, radius.
	//

	Point p1(0, 0);
	Point p2(2, 5);
	Point p4(-2, 5);
	Point p3(2, -5);
	Point p5(-2, -5);
	Color cRed(1, 0, 0);

	BresenhamLineFirstOctant(p1, p2, cRed);
	BresenhamLineLow(p1, p3, cRed);
	BresenhamLineLow(p1, p5, cRed);

	BresenhamLineHigh(p1, p2, cRed);
	BresenhamLineHigh(p1, p4, cRed);

	plotLine(p1, p3, cRed);
	plotLine(p1, p5, cRed);

	plotLine(p1, p2, cRed);
	plotLine(p1, p4, cRed);

	plotLine(p1, Point(-1, 20), cRed);

	int r = 15;					  // radius    of the circle
	Point p(-3, -5);			  // mid-point of the circle
	Color cBlue(0, 0, 1);		  // color     of the circle
	BresenhamCircle(p, r, cBlue); // plot the circle

	//
	// Please, do not change below this point!
	//
	//////////////////////////////////////////////////////////////////

	manageTexture();

	glClear(GL_COLOR_BUFFER_BIT);
	glBindTexture(GL_TEXTURE_2D, g_TexID);
	glEnable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);
	glColor3f(1, 0, 0);
	glTexCoord2f(0, 0);
	glVertex2f(-g_WinWidth / 2.0f, -g_WinHeight / 2.0f);
	glTexCoord2f(1, 0);
	glVertex2f(g_WinWidth / 2.0f, -g_WinHeight / 2.0f);
	glTexCoord2f(1, 1);
	glVertex2f(g_WinWidth / 2.0f, g_WinHeight / 2.0f);
	glTexCoord2f(0, 1);
	glVertex2f(-g_WinWidth / 2.0f, g_WinHeight / 2.0f);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	glFlush();
}

// The main
int main(int argc, char **argv)
{

	glutInit(&argc, argv);
	glutInitWindowSize(g_WinWidth, g_WinHeight);
	glutCreateWindow("Computer graphics exercise 1: Bresenham (WS25/26)");

	glutReshapeFunc(reshape); // is triggered on window size changes
	glutDisplayFunc(display); // is triggered to redraw the viewport/display

	glutMainLoop(); // start main loop

	glDeleteTextures(1, &g_TexID); // deletes the texture

	return 0;
}
