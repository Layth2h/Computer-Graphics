/////////////////////////////////////////////////////////////////////////////
//																									//
// Code framework for lecture computer graphics WS 2025/26 assignment 2+3	//
//																									//
// A generic class for n-dimensional coordinate vectors							//
//																									//
// (c) Georg Umlauf																			//
//																									//
/////////////////////////////////////////////////////////////////////////////

#include "Vector.h"
#include "Matrix.h"
#include "Planet.h"

#include <iostream>
#include <string>

// maybe you have to switch to your local include path, depending on your GLUT installation
#include "glut.h"

////////////////////////////////////////////////////////////
//
// system relevant global variables
//

// resolution of the of the main window (can be changed by the user)
int g_WinWidth = 800;
int g_WinHeight = 800;

// global variable to tune the timer interval
int g_iTimerMSecs;

//
/////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
//
// private, global variables ... REPLACE BY YOUR OWN ONES
//
// some global state variables used to describe ...
float g_iPos;	  // ... position and ...
float g_iPosIncr; // ... position increment (used in display1)

CVec2i g_vecPos;	 // same as above but in vector form ...
CVec2i g_vecPosIncr; // (used in display2)

// celestial bodies of exercise 3
Planet g_sun, g_planet, g_moon;

// angles (in degrees) of planet (around sun) and moon (around planet) ...
float g_planetAngle, g_moonAngle;
// ... and how much they grow per timer step
float g_planetAngleIncr, g_moonAngleIncr;

// what displayExercise3 shows: 1 = planet around sun (test),
// 2 = moon around fixed planet (test), 3 = both combined
int g_mode = 3;

// wanted frame rate (changed with the arrow keys) and fps measurement
float g_targetFps = 100;
int g_frames = 0;
int g_lastTime = 0;
//
/////////////////////////////////////////////////////////////

// Function to initialize our own variables
void init()
{
	// init timer interval
	g_iTimerMSecs = 10;

	// init variables for display1
	g_iPos = 0;
	g_iPosIncr = 2;

	// init variables for display2
	int aiPos[2] = {0, 0};
	int aiPosIncr[2] = {2, 2};
	g_vecPos.setData(aiPos);
	g_vecPosIncr.setData(aiPosIncr);

	// sun in the origin, planet and moon on the positive x-axis
	g_sun.setRadius(40);
	g_sun.setColor(Yellow);
	g_sun.setPosition(CVec2f(0.0f, 0.0f));

	g_planet.setRadius(20);
	g_planet.setColor(Blue);
	g_planet.setPosition(CVec2f(200.0f, 0.0f));

	g_moon.setRadius(8);
	g_moon.setColor(Green);
	g_moon.setPosition(CVec2f(260.0f, 0.0f));

	// start angles and angle increments
	g_planetAngle = 0;
	g_moonAngle = 0;
	g_planetAngleIncr = 1;
	g_moonAngleIncr = 3;
}

// Function to initialize the view to ortho-projection.
// Please do not change.
void initGL()
{
	glViewport(0, 0, g_WinWidth, g_WinHeight); // Establish viewport to cover entire window.

	glMatrixMode(GL_PROJECTION);			 // Start modifying the projection matrix.
	glLoadIdentity();						 // Init projection matrix.
	glOrtho(-g_WinWidth / 2, g_WinWidth / 2, // Map abstract coords directly to window coords.
			-g_WinHeight / 2, g_WinHeight / 2, 0, 1);
	glDrawBuffer(GL_BACK); // Tell GL that we draw to the back buffer and swap ...
						   // ... buffers when image is ready to avoid flickering
	float b = 70.0f / 100.0f;
	glClearColor(b, b, b, 1.0f); // Tell which color to use to clear image
}

// Callback Function to change the window size.
// Please do not change.
void reshape(int w, int h)
{
	g_WinWidth = w;
	g_WinHeight = h;
	glViewport(0, 0, w, h); // Establish viewport to cover entire window.

	glMatrixMode(GL_PROJECTION);				 // Start modifying the projection matrix.
	glLoadIdentity();							 // Reset projection matrix.
	glOrtho(-w / 2, w / 2, -h / 2, h / 2, 0, 1); // Map abstract coords directly to window coords.

	glutPostRedisplay();
}

// timer callback function
void timer(int value)
{
	///////
	// UPDATE YOUR VARIABLES HERE ...
	//
	g_planetAngle += g_planetAngleIncr;
	g_moonAngle += g_moonAngleIncr;

	//
	///////

	// These variables are just for the heaving triangles
	int size2 = std::min(g_WinWidth, g_WinHeight) / 2;

	// variables for displayExercise3 ... REPLACE BY YOUR OWN ONES
	if (g_iPos <= -size2 || g_iPos >= size2)
		g_iPosIncr = -g_iPosIncr;
	g_iPos += g_iPosIncr;

	// variables for displayExercise4 ... REPLACE BY YOUR OWN ONES
	if (g_vecPos[1] <= -size2 || g_vecPos[1] >= size2)
		g_vecPosIncr = -g_vecPosIncr;
	g_vecPos += g_vecPosIncr;

	// measure the real fps and show it in the window title once per second
	g_frames++;
	int now = glutGet(GLUT_ELAPSED_TIME);
	if (now - g_lastTime >= 1000)
	{
		std::string title = "Computer graphics assignment 2 - " + std::to_string(g_frames) + " fps (target " + std::to_string(int(g_targetFps)) + ")";
		glutSetWindowTitle(title.c_str());
		g_frames = 0;
		g_lastTime = now;
	}

	// the last two lines should always be
	glutPostRedisplay();
	glutTimerFunc(g_iTimerMSecs, timer, 0); // call timer for next iteration
}

// display callback function for EXERCISE 3
void displayExercise3(void)
{

	glClear(GL_COLOR_BUFFER_BIT);

	// start positions (the planets themselves are never changed)
	CVec2f planetStart = g_planet.getPosition();
	CVec2f moonStart = g_moon.getPosition();

	// work on copies, so the start positions stay intact
	Planet planet = g_planet;
	Planet moon = g_moon;

	if (g_mode == 1)
	{
		// test 1: planet rotates around the origin (sun)
		planet.setPosition(Planet::rotate(planetStart, g_planetAngle));
	}
	else if (g_mode == 2)
	{
		// test 2: moon rotates around the (fixed) planet
		moon.setPosition(Planet::rotate(moonStart, planetStart, g_moonAngle));
	}
	else
	{
		// combined: planet around sun ...
		planet.setPosition(Planet::rotate(planetStart, g_planetAngle));

		// ... moon first around the planet's start position, then
		// carried along with the planet around the sun
		CVec2f moonAroundPlanet = Planet::rotate(moonStart, planetStart, g_moonAngle);
		moon.setPosition(Planet::rotate(moonAroundPlanet, g_planetAngle));
	}

	g_sun.draw();
	planet.draw();
	moon.draw();

	// In double buffer mode the last two lines should always be
	glFlush();
	glutSwapBuffers(); // swap front and back buffer
}

// display callback function for EXERCISE 4
void displayExercise4(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	///////
	// remove the lines from "glBegin(...)" to "glEnd();"
	//
	// and display your planet data of EXERCISE 4 here
	// using the HOMOGENEOUS implementation of affine maps.
	//

	// start positions as homogeneous vectors (x, y, 1)
	CVec2f planetStart = g_planet.getPosition();
	CVec3f planetH(planetStart, 1.0f);
	CVec3f moonH(g_moon.getPosition(), 1.0f);

	// work on copies, so the start positions stay intact
	Planet planet = g_planet;
	Planet moon = g_moon;

	// planet around the sun (origin)
	CMat3f planetMat = Planet::rotationMatrix(g_planetAngle);

	// moon around the planet's start position (one matrix) ...
	CMat3f moonMat = Planet::rotationMatrix(planetStart, g_moonAngle);
	// ... then carried along with the planet around the sun
	moonMat = planetMat * moonMat;

	if (g_mode == 1)
		planet.setPosition(CVec2f(planetMat * planetH));	// test: only the planet moves
	else if (g_mode == 2)
		moon.setPosition(CVec2f(Planet::rotationMatrix(planetStart, g_moonAngle) * moonH));	// test: moon around fixed planet
	else
	{
		planet.setPosition(CVec2f(planetMat * planetH));
		moon.setPosition(CVec2f(moonMat * moonH));
	}

	g_sun.draw();
	planet.draw();
	moon.draw();

	//
	///////

	// In double buffer mode the last two lines should always be
	glFlush();
	glutSwapBuffers(); // swap front and back buffer
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'q':
	case 'Q':
		exit(0); // quit program
		break;
	case '1':
		glutDisplayFunc(displayExercise3);
		g_mode = 3; // combined
		break;
	case 'a':
		g_mode = 1; // test: planet around sun
		break;
	case 'b':
		g_mode = 2; // test: moon around planet
		break;
	case 'c':
		g_mode = 3; // planet and moon combined
		break;
	case '2':
		glutDisplayFunc(displayExercise4);
		break;
	default:
		// do nothing ...
		break;
	};
}

// arrow keys: up = faster, down = slower
// the wanted frame rate is doubled / halved, the timer interval follows from it
void special(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		g_targetFps += 5;
		break;
	case GLUT_KEY_DOWN:
		g_targetFps = std::max(1.0f, g_targetFps - 5);
		break;
	default:
		return; // other special keys: nothing to report
	}
	g_iTimerMSecs = int(1000.0f / g_targetFps); // above 1000 fps this is 0 = as fast as possible
	std::cout << "target: " << g_targetFps << " fps" << std::endl;
}

// The main
// Please do not change.
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("Computer graphics assignment 2: 2d-Rotations (WS25/26)");
	glutReshapeWindow(g_WinWidth, g_WinHeight);

	init();	  // init my variables first
	initGL(); // init the GL (i.e. view settings, ...)

	glutTimerFunc(10, timer, 0);
	glutReshapeFunc(reshape);		   // is triggered on window size changes
	glutDisplayFunc(displayExercise3); // is triggered to redraw the viewport/display
	glutKeyboardFunc(keyboard);		   // is triggered on keyboard events
	glutSpecialFunc(special);		   // is triggered on arrow keys

	// start main loop
	glutMainLoop();

	return 0;
}
