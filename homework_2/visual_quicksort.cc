// Programmer: Michael Chyziak
// Date: Wednesday March 17, 2018
//
// Shows the result of the quicksort algorithm using two examples. 
// For one of the examples, the letters of the programmers full name is sorted in ASCII value. 
// The other example sorts and stacks 4 graphical blocks according to their R value in their RGB color deceleration. 
// The initial screen is the user specified colors of the random stack of block and the final screen is the sorted assortment of the RGB blocks.
// A terminal interface is used for the user inputs.
//
// g++ visual_quicksort.cc -lm -lglut -lGL -lGLU -o visual_quicksort

// Standard libraries
#include <iostream>
#include <string>
#include <stdlib.h>
#include <cstring>
#include <math.h> // fabs()

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

// Function Declarations
void drawObjects();
void drawCube(float startX, float startY, float startZ, 
		float sizeX, float sizeY, float sizeZ,
		float angleX, float angleY, float angleZ,
		int colourRed, int colourGreen, int colourBlue);
void cubeSort(int argc, char **argv);
void nameSort(const char* name_unsorted);
void quickSort(int array[], int left_start, int right_start);
int inputToInt(string user_input);
void resetCubeCoordinates();
void detectKeyboard(unsigned char key, int x, int y);
void updateDisplay(int timer_id);

// Global coordinates for all 4 cubes (only y needed)
float cubeYCoordinate [4];

// Global RGB colour values of the four cubes
int cubeColours[4][3];

// Global cube sort order
// nth array index stands for nth+1 cube position
// position can be anywhere from 0 to 3 (top to bottom)
int cubeOrder[4];

// Global for the status of the GUI
enum guiStatus {UNSORTED, SORTING, SORTED, RESET};
guiStatus currentStatus = UNSORTED;

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
					resetCubeCoordinates();
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

// Resets the coordinates of the 4 cubes  
void resetCubeCoordinates() {	

	// Variables
	int cube_index;
	// Starting value of first cube (bottom)
	float cube_value_y_val = -0.675;

	// Reset coordinates for all 4 cubes 
	for (cube_index = 0; cube_index < 4; cube_index++) {
		cubeYCoordinate[cube_index] = cube_value_y_val;
		cube_value_y_val += 0.45;
	}
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

// Sorts 4 cubes with user diffened RGB values
// Cubes are sorted in order of highest to lowest R values
// Sorting is done in OpenGL by pressing a button to start sorting
// A reset button can be pressed to sort again
// A close button or closing the window closes the OpenGL view
// Cubes phase through each other into the proper sorted position
void cubeSort(int argc, char **argv) {

	// Variables
	int cube_index;
	int colour_index;
	string colour_string;
	string user_input;
	int user_input_int;
	int cubesRed[4];
	bool cubeTaken[4];

	// Example explanation/initialization
	printf("Four cubes will be shown on screen with custom RGB values.\n");
	printf("You will be asked to input the RGB values next.\n");
	printf("The intensity of a colour can be input from 0 to 100.\n");
	printf("Cubes are initialized from bottom to top.\n");
	
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
				cubeColours[cube_index][colour_index] = 
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

	// Initialize cube red inputs into array
	// Initialize cube taken array to all false
	for (cube_index = 0; cube_index < 4; cube_index++) {
		cubesRed[cube_index] = cubeColours[cube_index][0];
		cubeTaken[cube_index] = false;
	}
	
	// quicksort algorithm
	quickSort(cubesRed, 0, 3);

	// Initialize cubeOrder to unknown values
	for (cube_index = 0; cube_index < 4; cube_index++) {
		cubeOrder[cube_index] = -1;
	}

	// Get proper order of cubes from 0 to 3
	for (cube_index = 0; cube_index < 4; cube_index++) {
		for (colour_index = 0; colour_index < 4; colour_index++) {
			if (cubeColours[cube_index][0] == cubesRed[colour_index]) {
				printf("here\n");
				if (cubeTaken[colour_index] == false) {
					cubeOrder[cube_index] = colour_index;
					cubeTaken[colour_index] = true;
					break;
				}
			}
		}
	}

	// Instruction message
	printf("Press *ENTER* to start sorting or *ESC* to return to terminal.\n");

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
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	// Callback functions
	glutDisplayFunc(drawObjects);
	glutKeyboardFunc(detectKeyboard);
	glutTimerFunc(1000.0/60.0, updateDisplay, 0);

	// Pass control to GLUT for events
        glutMainLoop();
}

// Update the display to match the sorted cube quicksort algorithm
// Cubes will move slowly into place over time
// The status will change when they have all reached their correct order
// Ordering is from top to bottom (highest R values at top)
void updateDisplay(int timer_id) {
	
	// Variables
	float interval = 0.45;
	float starting = -0.675;
	int index;
	float cubeFinalPos;
	float cubeCurrentPos;
	int cubesSorted = 0;
	float epsilon = 0.01;

	// Update cube values
	if (currentStatus == SORTING || currentStatus == RESET) {
		for (index = 0; index < 4; index++) {

			// DEBUG
			// printf("cubeOrder[%d]: %d\n", index, cubeOrder[index]);

			// Cube current and final positions
			if (currentStatus == SORTING) {
				cubeFinalPos = starting + (interval * cubeOrder[index]);
			}
			else { //currentStatus == RESET
				cubeFinalPos = starting + (interval * index);
			}
			cubeCurrentPos = cubeYCoordinate[index];

			// DEBUG
			// printf("Final[%d]: %f\n", index, cubeFinalPos);
			// printf("Current[%d]: %f\n", index, cubeCurrentPos);

			// Use epsilon to see if floats are "equal" 
			if (fabs(cubeCurrentPos - cubeFinalPos) < epsilon) {
				cubesSorted++;
			}
			else if (cubeCurrentPos < cubeFinalPos) {
				cubeYCoordinate[index] += 0.05;
			}
			else { // cubeCurrentPos > cubeFinalPos
				cubeYCoordinate[index] -= 0.05;
			}
		}

		// Change status if all cubes have been sorted
		if (cubesSorted == 4) {
			if (currentStatus == SORTING) {
				printf("Sorting complete.\n");
				printf("Press *ENTER* to reset to original configuration or *ESC* to return to terminal.\n");
				currentStatus = SORTED;
			}
			else { //currentStatus == RESET
				printf("Reset complete.\n");
				printf("Press *ENTER* to start sorting or *ESC* to return to terminal.\n");
				currentStatus = UNSORTED;
			}
		}
	}

	// Redisplay
	glutPostRedisplay();

	// Setup next timer	
	glutTimerFunc(1000.0/60.0, updateDisplay, 0);
}

// Continue with sorting when Enter is pressed
// If sorting is done, Enter will reset cubes to initial states
// Pressing escape closes the GUI
void detectKeyboard(unsigned char key, int x, int y) {

	// Enter button pressed, change sorting status
	if (key == 13) {
		if (currentStatus == UNSORTED) {
			printf("Sorting...\n");
			currentStatus = SORTING;
		}
		else if (currentStatus == SORTED) {
			printf("Resetting...\n");
			currentStatus = RESET;
			resetCubeCoordinates();
		}
	}

	// If escape is pressed, quit the GUI
	if (key == 27) {
		printf("Closing GUI.\n");
		glutLeaveMainLoop();
		resetCubeCoordinates();
		currentStatus = UNSORTED;
	}
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

// Draws the four cubes on screen
// Cubes are either unsorted, being sorted, or already sorted
// Text is also displayed to let the user know the state of the sorting
void drawObjects() {

	// Set Background Color to a light grey
	glClearColor(0.4, 0.4, 0.4, 1.0);

 	// Clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    	// Reset transformations
	glMatrixMode(GL_MODELVIEW);
    	glLoadIdentity();

	// Draw the four cubes
	drawCube(0, cubeYCoordinate[0], 0, 0.3, 0.3, 0.3, -20, 20, 0, 
			cubeColours[0][0], cubeColours[0][1], 
			cubeColours[0][2]);
	drawCube(0, cubeYCoordinate[1], 0, 0.3, 0.3, 0.3, -20, 20, 0, 
			cubeColours[1][0], cubeColours[1][1], 
			cubeColours[1][2]);
	drawCube(0, cubeYCoordinate[2], 0, 0.3, 0.3, 0.3, -20, 20, 0, 
			cubeColours[2][0], cubeColours[2][1], 
			cubeColours[2][2]);
	drawCube(0, cubeYCoordinate[3], 0, 0.3, 0.3, 0.3, -20, 20, 0, 
			cubeColours[3][0], cubeColours[3][1], 
			cubeColours[3][2]);

	// Swap to buffer
	glFlush();
	glutSwapBuffers();
}

// Draws a cube according to the size and location parameters
// Color is also based on the set parameters
// start location is middle of cube
// Colours are given from 0 to 100 in terms of intensity
void drawCube(float startX, float startY, float startZ, 
		float sizeX, float sizeY, float sizeZ,
		float angleX, float angleY, float angleZ,
		int colourRed, int colourGreen, int colourBlue) {
	
	// Variables
	float dx = sizeX/2;
	float dy = sizeY/2;
	float dz = sizeZ/2;
	// Convert int colour values to corresponding floats
	float red = colourRed/100.0;
	float green = colourGreen/100.0;
	float blue = colourBlue/100.0;

	glPushMatrix();

	// Rotate cube accordingly
	glRotatef(angleX, 1.0, 0.0, 0.0);
	glRotatef(angleY, 0.0, 1.0, 0.0);
	glRotatef(angleZ, 0.0, 0.0, 1.0);

	// Front
    	glBegin(GL_POLYGON);
    	glColor3f(red, green, blue);
        glVertex3f(startX-dx, startY+dy, startZ-dz);
        glVertex3f(startX+dx, startY+dy, startZ-dz);
        glVertex3f(startX+dx, startY-dy, startZ-dz);
        glVertex3f(startX-dx, startY-dy, startZ-dz);
    	glEnd();

	// Left
        glBegin(GL_POLYGON);
    	glColor3f(red, green, blue);
        glVertex3f(startX-dx, startY+dy, startZ+dz);
        glVertex3f(startX-dx, startY+dy, startZ-dz);
        glVertex3f(startX-dx, startY-dy, startZ-dz);
        glVertex3f(startX-dx, startY-dy, startZ+dz);
        glEnd();

        // Right
	// Mimic light by making 60% of front colour
        glBegin(GL_POLYGON);
    	glColor3f(red*0.6, green*0.6, blue*0.6);
        glVertex3f(startX+dx, startY+dy, startZ+dz);
        glVertex3f(startX+dx, startY+dy, startZ-dz);
        glVertex3f(startX+dx, startY-dy, startZ-dz);
        glVertex3f(startX+dx, startY-dy, startZ+dz);
        glEnd();

  	// Top
	// Mimic light by making 80% of front colour
        glBegin(GL_POLYGON);
    	glColor3f(red*0.8, green*0.8, blue*0.8);
        glVertex3f(startX+dx, startY+dy, startZ+dz);
        glVertex3f(startX+dx, startY+dy, startZ-dz);
        glVertex3f(startX-dx, startY+dy, startZ-dz);
        glVertex3f(startX-dx, startY+dy, startZ+dz);
        glEnd();

        // Bottom
        glBegin(GL_POLYGON);
    	glColor3f(red, green, blue);
        glVertex3f(startX+dx, startY-dy, startZ+dz);
        glVertex3f(startX+dx, startY-dy, startZ-dz);
        glVertex3f(startX-dx, startY-dy, startZ-dz);
        glVertex3f(startX-dx, startY-dy, startZ+dz);
        glEnd();

	// Back
        glBegin(GL_POLYGON);
    	glColor3f(red, green, blue);
        glVertex3f(startX-dx, startY+dy, startZ+dz);
        glVertex3f(startX+dx, startY+dy, startZ+dz);
        glVertex3f(startX+dx, startY-dy, startZ+dz);
        glVertex3f(startX-dx, startY-dy, startZ+dz);
        glEnd();
	
	glPopMatrix();
}

