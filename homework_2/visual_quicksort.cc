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
void drawCube(float startX, float startY, float startZ, 
		float sizeX, float sizeY, float sizeZ,
		float angleX, float angleY, float angleZ,
		float colourRed, float colourGreen, float colourBlue);
void cubeSort(int argc, char **argv);
void nameSort(const char* name_unsorted);
void quickSort(int array[], int left_start, int right_start);
int inputToInt(string user_input);

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

	// Variables
	int cube_colours[4][3];
	int cube_index;
	int colour_index;
	string colour_string;
	string user_input;
	int user_input_int;

	// Example explanation/initialization
	printf("Four cubes will be shown on screen with custom RGB values.\n");
	printf("You will be asked to input the RGB values next.\n");
	printf("The intensity of a colour can be input from 0 to 100.\n");
	
	// Get user input
	for (cube_index = 0; cube_index < 4; cube_index++) {
		for (colour_index = 0; colour_index < 3; colour_index++) {
			if (colour_index == 0) {
				colour_string = "red";
			}
			else if (colour_index == 1) {
				colour_string = "green";
			}
			else {
				colour_string = "blue";
			}
			printf("Cube %d (%s intensity): ", cube_index + 1, 
				colour_string.c_str());
			cin >> user_input;

			// Check if user input is a number between 0 and 100
			user_input_int = inputToInt(user_input);
			if (user_input_int >= 0 && 
				user_input_int <= 100) {
				cube_colours[cube_index][colour_index] = 
					user_input_int;
			}
			else {
				// Invalid input
				printf("Error: invalid user input. Try "
					"again.\n");
				colour_index--;
			}
		}
	}

	// Initialize GLUT and process user parameters
	glutInit(&argc, argv);

	// Request double buffered true colour window with Z-buffer
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

	// Pass control to GLUT for events
        glutMainLoop();
}

// Checks if a string can be converted to integer values.
// Only integer values from 0 to 100 are accepted and then returned.
// Otherwise -1 will be returned.
int inputToInt(string user_input) {
	
	// Variables
	int input_index;

	// Check if input more than 3 characters then return -1
	if (user_input.length() > 3) {
		return -1;	
	}
	
	// Check if input is a valid number
	// Valid number is ascii value from 48 to 57
	// Return -1 if not valid
	for (input_index = 0; input_index < user_input.length();
		input_index++) {
		if ((user_input.at(input_index) < 48) ||
			(user_input.at(input_index) > 57)) {
			return -1;
		}
	}

	// Convert valid string in integer format to integer
	return stoi(user_input, nullptr);
}

// Uses the quicksort algorithm on an integer array with a left and right
// starting point.
// Used recursively and stores the final "sorted" array in the array given
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

// Draws the cube, pyramid, and diamond
void drawObjects() {

	// TODO

	// Set Background Color to a light grey
	glClearColor(0.4, 0.4, 0.4, 1.0);

 	// Clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    	// Reset transformations
	glMatrixMode(GL_MODELVIEW);
    	glLoadIdentity();

	// Draw cube, pyramid, and/or diamond based on current selection
	if (currentObject == all) {
		drawCube(-0.6, 0, 0, 0.4, 0.4, 0.4, -20, 20, 0, 1.0, 0, 0);
	}
	else if (currentObject == cube) {
		drawCube(0, 0, 0, 0.8, 0.8, 0.8, -20, 20, 0, 1.0, 0, 0);
	}

	glFlush();
	glutSwapBuffers();
}

// Draws a cube according to the size and location parameters
// Color is also based on the set parameters
// start location is middle of cube
void drawCube(float startX, float startY, float startZ, 
		float sizeX, float sizeY, float sizeZ,
		float angleX, float angleY, float angleZ,
		float colourRed, float colourGreen, float colourBlue) {
	
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
    	glColor3f(colourRed, colourGreen, colourBlue);
        glVertex3f(startX-dx, startY+dy, startZ-dz);
        glVertex3f(startX+dx, startY+dy, startZ-dz);
        glVertex3f(startX+dx, startY-dy, startZ-dz);
        glVertex3f(startX-dx, startY-dy, startZ-dz);
    	glEnd();

	// Left
        glBegin(GL_POLYGON);
    	glColor3f(colourRed, colourGreen, colourBlue);
        glVertex3f(startX-dx, startY+dy, startZ+dz);
        glVertex3f(startX-dx, startY+dy, startZ-dz);
        glVertex3f(startX-dx, startY-dy, startZ-dz);
        glVertex3f(startX-dx, startY-dy, startZ+dz);
        glEnd();

        // Right
        glBegin(GL_POLYGON);
    	glColor3f(colourRed, colourGreen, colourBlue);
        glVertex3f(startX+dx, startY+dy, startZ+dz);
        glVertex3f(startX+dx, startY+dy, startZ-dz);
        glVertex3f(startX+dx, startY-dy, startZ-dz);
        glVertex3f(startX+dx, startY-dy, startZ+dz);
        glEnd();

  	// Top
        glBegin(GL_POLYGON);
    	glColor3f(colourRed, colourGreen, colourBlue);
        glVertex3f(startX+dx, startY+dy, startZ+dz);
        glVertex3f(startX+dx, startY+dy, startZ-dz);
        glVertex3f(startX-dx, startY+dy, startZ-dz);
        glVertex3f(startX-dx, startY+dy, startZ+dz);
        glEnd();

        // Bottom
        glBegin(GL_POLYGON);
    	glColor3f(colourRed, colourGreen, colourBlue);
        glVertex3f(startX+dx, startY-dy, startZ+dz);
        glVertex3f(startX+dx, startY-dy, startZ-dz);
        glVertex3f(startX-dx, startY-dy, startZ-dz);
        glVertex3f(startX-dx, startY-dy, startZ+dz);
        glEnd();

	// Back
        glBegin(GL_POLYGON);
    	glColor3f(colourRed, colourGreen, colourBlue);
        glVertex3f(startX-dx, startY+dy, startZ+dz);
        glVertex3f(startX+dx, startY+dy, startZ+dz);
        glVertex3f(startX+dx, startY-dy, startZ+dz);
        glVertex3f(startX-dx, startY-dy, startZ+dz);
        glEnd();
	
	glPopMatrix();
}

