// Programmer: Michael Chyziak
// Date: Wednesday March 17, 2018
//
// TODO: program definition
//
// g++ visual_quicksort.cc -lm -lglut -lGL -lGLU -o visual_quicksort

// Standard libraries
#include <iostream>
#include <string>
#include <stdlib.h>
#include <cstring>

// Link GLUT and OpenGL libraries
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// Link freeglut (for stopping the program to end when GLUT window closes)
#include <GL/freeglut.h>

using namespace std;

void drawObjects();
void keyboardSpecial(int key, int x, int y);
void drawCube(float startX, float startY, float startZ, 
		float sizeX, float sizeY, float sizeZ,
		float angleX, float angleY, float angleZ);
void cubeSort(int argc, char **argv);
void nameSort(const char* name_unsorted);
void quickSort(int array[], int left_start, int right_start);

// The current object being shown on the screen
enum screenObject {all, cube};
screenObject currentObject = all;


int main(int argc, char **argv) {

	// Variables
	string user_input;
	int user_input_size;
	bool run_program = true;

	// Run program while running condition is met
	while(run_program == true) {

		// Program explanation/initialization
		printf("Enter \'1\' to see the sorting of my full name.\n");
		printf("Enter \'2\' to see the sorting of 4 cubes on screen "
			" according to their rgb colours.\n");
		printf("Enter \'3\' to close the program.\n");
	
		// Get user input
		printf("User Selection: ");
		cin >> user_input;

		// Check if input is valid
		if (user_input.length() == 1) {
			switch (user_input.at(0)) {
				case '1':
					// Name sort
					nameSort("MICHAEL CHYZIAK");
					break;
				case '2':
					// OpenGL Cube Sort
					cubeSort(argc, argv);
					break;
				case '3':
					// Close program
					printf("Closing program.\n");
					run_program = false;
					break;
				default:
					// Invalid input
					printf("Error: invalid user input. Try "
						"again.\n");
					break;
			}
		}
		else {
			// Invalid input
			printf("Error: invalid user input. Try again.\n");
		}
	}
	
	return 0;
}

// Sorts the input argument in ascii order and prints the sorted name.
// *space* = 32
// A = 65
// B = 66
// C = 67
// etc.
void nameSort(const char* name_unsorted) {
	
	// Variables
	int index;
	int name_unsorted_length = strlen(name_unsorted);
	int name_array[name_unsorted_length];

	// Convert string to int array
	for (index = 0; index < name_unsorted_length; index++) {
		name_array[index] = name_unsorted[index];
	}
	
	// quicksort algorithm
	quickSort(name_array, 0, name_unsorted_length-1);

	// Print output
	printf("Original name: %s\n", name_unsorted);
	for (index = 0; index < name_unsorted_length; index++) {
		printf("%c", (char) name_array[index]);
	}
	printf("\n");

}

// TODO
void cubeSort(int argc, char **argv) {

	// Initialize GLUT and process user parameters
	glutInit(&argc, argv);

	// Request double buffered true color window with Z-buffer
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	// Create window
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("ENSC482 Michael Chyziak - Homework 2");
	
	// Update depth buffer if test passes
        glEnable(GL_DEPTH_TEST);

	// Don't exit when GLUT window closed (uses freeglut.h)
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, 
			GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	// Callback functions
	glutDisplayFunc(drawObjects);
	glutSpecialFunc(keyboardSpecial);

	// Pass control to GLUT for events
        glutMainLoop();
}

// TODO
void quickSort(int array[], int left_start, int right_start) {
	
	// Variables
	int left_index = left_start;
	int right_index = right_start;
	int temp_value;
	// Get pivot value from index in centre of array
	int pivot_value = array[(left_start + right_start) / 2];

	// Partition
	while (left_index <= right_index) {
		while (array[left_index] < pivot_value) {
			left_index++;
		}
		while (array[right_index] > pivot_value) {
			right_index--;
		}
		if (left_index <= right_index) {
			temp_value = array[left_index];
			array[left_index] = array[right_index];
			array[right_index] = temp_value;
			left_index++;
			right_index--;
		}
	}

	// Recursion
	if (left_start < right_index) {
		quickSort(array, left_start, right_index);
	}
	if (left_index < right_start) {
		quickSort(array, left_index, right_start);
	}

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
				currentObject = all;
				break;
		}
	}
	else if (key == GLUT_KEY_LEFT) {
		switch(currentObject) {
			case all:
				currentObject = cube;
				break;
			case cube:
				currentObject = all;
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
	}
	else if (currentObject == cube) {
		drawCube(0, 0, 0, 0.8, 0.8, 0.8, -20, 20, 0);
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

