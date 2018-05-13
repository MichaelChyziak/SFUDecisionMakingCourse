// Programmer: Michael Chyziak
// Date: Wednesday March 9, 2018
//
// Shows a solid cube, pyramid, and diamond or all 3 depending on 
// keyboard press.
//
// g++ visualization_model.cc -lm -lglut -lGL -lGLU -o visualization_model

#include <iostream>
#include <string>
#include <stdlib.h>

// Link GLUT and OpenGL libraries
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;

void drawObjects();
void keyboardSpecial(int key, int x, int y);
void drawCube(float startX, float startY, float startZ, 
		float sizeX, float sizeY, float sizeZ,
		float angleX, float angleY, float angleZ);
void drawPyramid(float startX, float startY, float startZ, 
		float sizeX, float sizeY, float sizeZ,
		float angleX, float angleY, float angleZ);
void drawDiamond(float startX, float startY, float startZ,	
		float sizeX, float sizeY, float sizeZ,
		float angleX, float angleY, float angleZ);

// The current object being shown on the screen
enum screenObject {all, cube, pyramid, diamond};
screenObject currentObject = all;


int main(int argc, char **argv) {

	// Initialize GLUT and process user parameters
	glutInit(&argc, argv);

	// Request double buffered true color window with Z-buffer
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	// Create window
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("ENSC482 Michael Chyziak - Homework 1");
	
	// TODO what does this do?
        glEnable(GL_DEPTH_TEST);

	// Callback functions
	glutDisplayFunc(drawObjects);
	glutSpecialFunc(keyboardSpecial);

	// Pass control to GLUT for events
        glutMainLoop();
	return 0;
}


// Gets the user special input and changes the current object shown
// The scene cycles between all 3 cube, pyramid, and diamond being shown (all)
// The sequence is all -> cube -> pyramid -> diamond -> all for right arrow
// The sequence is all -> diamond -> pyramid -> cube -> all for left arrow
void keyboardSpecial(int key, int x, int y) {

	// Checks what key is pressed and changes the current object 
	// shown on screen if it is the left or right arrow
	if (key == GLUT_KEY_RIGHT) {
		switch(currentObject) {
			case all:
				currentObject = cube;
				break;
			case cube:
				currentObject = pyramid;
				break;
			case pyramid:
				currentObject = diamond;
				break;
			case diamond:
				currentObject = all;
				break;
		}
	}
	else if (key == GLUT_KEY_LEFT) {
		switch(currentObject) {
			case all:
				currentObject = diamond;
				break;
			case cube:
				currentObject = all;
				break;
			case pyramid:
				currentObject = cube;
				break;
			case diamond:
				currentObject = pyramid;
				break;
		}
	}

	// Request display to update
	glutPostRedisplay();

}

// Draws the cube, pyramid, and diamond
void drawObjects() {

	// Set Background Color to a light grey
	glClearColor(0.4, 0.4, 0.4, 1.0);

 	// Clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    	// Reset transformations
	glMatrixMode(GL_MODELVIEW);
    	glLoadIdentity();

	// Draw cube, pyramid, and/or diamond based on current selection
	if (currentObject == all) {
		drawCube(-0.6, 0, 0, 0.4, 0.4, 0.4, -20, 20, 0);
		drawPyramid(0, 0, 0, 0.4, 0.4, 0.4, -20, 20, 0); 
		drawDiamond(0.6, 0, 0, 0.4, 0.8, 0.4, -20, 20, 0);
	}
	else if (currentObject == cube) {
		drawCube(0, 0, 0, 0.8, 0.8, 0.8, -20, 20, 0);
	}
	else if (currentObject == pyramid) {
		drawPyramid(0, 0, 0, 0.8, 0.8, 0.8, -20, 20, 0); 
	}
	else if (currentObject == diamond) {
		drawDiamond(0, 0, 0, 0.8, 1.6, 0.8, -20, 20, 0);
	}

	glFlush();
	glutSwapBuffers();
}

// Draws a red cube according to the size and location parameters
// start location is middle of cube
void drawCube(float startX, float startY, float startZ, 
		float sizeX, float sizeY, float sizeZ,
		float angleX, float angleY, float angleZ) {
	
	float dx = sizeX/2;
	float dy = sizeY/2;
	float dz = sizeZ/2;

	glPushMatrix();

	// Rotate cube accordingly
	glRotatef(angleX, 1.0, 0.0, 0.0);
	glRotatef(angleY, 0.0, 1.0, 0.0);
	glRotatef(angleZ, 0.0, 0.0, 1.0);

	// Front
    	glBegin(GL_POLYGON);
    	glColor3f(1.0, 0.0, 0.0);
        glVertex3f(startX-dx, startY+dy, startZ-dz);
        glVertex3f(startX+dx, startY+dy, startZ-dz);
        glVertex3f(startX+dx, startY-dy, startZ-dz);
        glVertex3f(startX-dx, startY-dy, startZ-dz);
    	glEnd();

	// Left
        glBegin(GL_POLYGON);
        glColor3f(0.9, 0.0, 0.0);
        glVertex3f(startX-dx, startY+dy, startZ+dz);
        glVertex3f(startX-dx, startY+dy, startZ-dz);
        glVertex3f(startX-dx, startY-dy, startZ-dz);
        glVertex3f(startX-dx, startY-dy, startZ+dz);
        glEnd();

        // Right
        glBegin(GL_POLYGON);
        glColor3f(0.8, 0.0, 0.0);
        glVertex3f(startX+dx, startY+dy, startZ+dz);
        glVertex3f(startX+dx, startY+dy, startZ-dz);
        glVertex3f(startX+dx, startY-dy, startZ-dz);
        glVertex3f(startX+dx, startY-dy, startZ+dz);
        glEnd();

  	// Top
        glBegin(GL_POLYGON);
        glColor3f(0.7, 0.0, 0.0);
        glVertex3f(startX+dx, startY+dy, startZ+dz);
        glVertex3f(startX+dx, startY+dy, startZ-dz);
        glVertex3f(startX-dx, startY+dy, startZ-dz);
        glVertex3f(startX-dx, startY+dy, startZ+dz);
        glEnd();

        // Bottom
        glBegin(GL_POLYGON);
        glColor3f(0.6, 0.0, 0.0);
        glVertex3f(startX+dx, startY-dy, startZ+dz);
        glVertex3f(startX+dx, startY-dy, startZ-dz);
        glVertex3f(startX-dx, startY-dy, startZ-dz);
        glVertex3f(startX-dx, startY-dy, startZ+dz);
        glEnd();

	// Back
        glBegin(GL_POLYGON);
        glColor3f(0.5, 0.0, 0.0);
        glVertex3f(startX-dx, startY+dy, startZ+dz);
        glVertex3f(startX+dx, startY+dy, startZ+dz);
        glVertex3f(startX+dx, startY-dy, startZ+dz);
        glVertex3f(startX-dx, startY-dy, startZ+dz);
        glEnd();
	
	glPopMatrix();
}


// Draws a green pyramid according to the size and location parameters
// start location is middle of pyramid
void drawPyramid(float startX, float startY, float startZ, 
		float sizeX, float sizeY, float sizeZ,
		float angleX, float angleY, float angleZ) {

	float dx = sizeX/2;
	float dy = sizeY/2;
	float dz = sizeZ/2;

	glPushMatrix();

	// Rotate pyramid accordingly
	glRotatef(angleX, 1.0, 0.0, 0.0);
	glRotatef(angleY, 0.0, 1.0, 0.0);
	glRotatef(angleZ, 0.0, 0.0, 1.0);

	// Front
	glBegin(GL_TRIANGLES);
        glColor3f(0.0, 1.0, 0.0);
        glVertex3f(startX+dx, startY-dy, startZ-dz);
        glVertex3f(startX-dx, startY-dy, startZ-dz);
	glVertex3f(startX, startY+dy, startZ);
        glEnd();

	// Left
        glBegin(GL_TRIANGLES);
    	glColor3f(0.0, 0.9, 0.0);
        glVertex3f(startX-dx, startY-dy, startZ+dz);
        glVertex3f(startX-dx, startY-dy, startZ-dz);
	glVertex3f(startX, startY+dy, startZ);
    	glEnd();

	// Right side
        glBegin(GL_TRIANGLES);
        glColor3f(0.0, 0.8, 0.0);
        glVertex3f(startX+dx, startY-dy, startZ+dz);
        glVertex3f(startX+dx, startY-dy, startZ-dz);
	glVertex3f(startX, startY+dy, startZ);
        glEnd();

        // Bottom
        glBegin(GL_POLYGON);
        glColor3f(0.0, 0.7, 0.0);
        glVertex3f(startX+dx, startY-dy, startZ+dz);
        glVertex3f(startX-dx, startY-dy, startZ+dz);
        glVertex3f(startX-dx, startY-dy, startZ-dz);
        glVertex3f(startX+dx, startY-dy, startZ-dz);
        glEnd();

	// Back
        glBegin(GL_TRIANGLES);
        glColor3f(0.0, 0.6, 0.0);
        glVertex3f(startX+dx, startY-dy, startZ+dz);
        glVertex3f(startX-dx, startY-dy, startZ+dz);
	glVertex3f(startX, startY+dy, startZ);
        glEnd();
	
	glPopMatrix();

}

// Draws a blue diamond according to the size and location parameters
// start location is middle of diamond
void drawDiamond(float startX, float startY, float startZ, 
		float sizeX, float sizeY, float sizeZ,
		float angleX, float angleY, float angleZ) {

	float dx = sizeX/2;
	float dy = sizeY/2;
	float dz = sizeZ/2;
	
	glPushMatrix();
	
	// Rotate diamond accordingly
	glRotatef(angleX, 1.0, 0.0, 0.0);
	glRotatef(angleY, 0.0, 1.0, 0.0);
	glRotatef(angleZ, 0.0, 0.0, 1.0);

	// Front Top	
        glBegin(GL_TRIANGLES);
        glColor3f(0.0, 0.0, 1.0);
        glVertex3f(startX+dx, startY, startZ-dz);
        glVertex3f(startX-dx, startY, startZ-dz);
	glVertex3f(startX, startY+dy, startZ);
        glEnd();
	
	// Front Bottom	
        glBegin(GL_TRIANGLES);
        glColor3f(0.0, 0.0, 0.9);
        glVertex3f(startX+dx, startY, startZ-dz);
        glVertex3f(startX-dx, startY, startZ-dz);
	glVertex3f(startX, startY-dy, startZ);
        glEnd();
	
	// Left Top	
        glBegin(GL_TRIANGLES);
        glColor3f(0.0, 0.0, 0.8);
        glVertex3f(startX-dx, startY, startZ+dz);
        glVertex3f(startX-dx, startY, startZ-dz);
	glVertex3f(startX, startY+dy, startZ);
        glEnd();
	
	// Left Bottom	
        glBegin(GL_TRIANGLES);
        glColor3f(0.0, 0.0, 0.7);
        glVertex3f(startX-dx, startY, startZ+dz);
        glVertex3f(startX-dx, startY, startZ-dz);
	glVertex3f(startX, startY-dy, startZ);
        glEnd();
	
	// Right Top	
        glBegin(GL_TRIANGLES);
        glColor3f(0.0, 0.0, 0.6);
        glVertex3f(startX+dx, startY, startZ+dz);
        glVertex3f(startX+dx, startY, startZ-dz);
	glVertex3f(startX, startY+dy, startZ);
        glEnd();
	
	// Right Bottom	
        glBegin(GL_TRIANGLES);
        glColor3f(0.0, 0.0, 0.5);
        glVertex3f(startX+dx, startY, startZ+dz);
        glVertex3f(startX+dx, startY, startZ-dz);
	glVertex3f(startX, startY-dy, startZ);
        glEnd();
	
	// Back Top	
        glBegin(GL_TRIANGLES);
        glColor3f(0.0, 0.0, 0.4);
        glVertex3f(startX+dx, startY, startZ+dz);
        glVertex3f(startX-dx, startY, startZ+dz);
	glVertex3f(startX, startY+dy, startZ);
        glEnd();

	// Back Bottom	
        glBegin(GL_TRIANGLES);
        glColor3f(0.0, 0.0, 0.3);
        glVertex3f(startX+dx, startY, startZ+dz);
        glVertex3f(startX-dx, startY, startZ+dz);
	glVertex3f(startX, startY-dy, startZ);
        glEnd();
	
	glPopMatrix();
}
