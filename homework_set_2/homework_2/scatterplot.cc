// Programmer: Michael Chyziak
// Date: Wednesday June 30, 2018
//
// Program description:
// 	TODO
// Required supplementary files:
// 	wine.data
// Got data set from here: 
//	http://archive.ics.uci.edu/ml/machine-learning-databases/wine/wine.data
// g++ scatterplot.cc -lm -lglut -lGL -lGLU -o scatterplot

// Standard libraries
#include <iostream>
#include <string>
#include <stdlib.h>
#include <cstring>
#include <math.h>

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
int parseWineData(float wine_data[178][3]);
void detectKeyboard(unsigned char key, int x, int y);
void displayGUI(int argc, char **argv);
void drawScreen();
void displayString(float x, float y, string input);

// Global Variables
float wine_data[178][3]; // Know that there is exactly 178 data points with 3 variables (alcohol, colour intensity, and hue)

int main(int argc, char **argv) {

	// Variables
	int status;

	// Parses input from file and stores into user_data array
	// If cannot read from file it ends the program
	status = parseWineData(wine_data);
	if (status == -1) {
		return status;		
	}
	
	// Calculate and display (printf) 3x3 correlation coefficiants and final correclation coefficiant
	// TODO

	// Run GUI
	displayGUI(argc, argv);
}

// Parses file to get the appropriate wine data on the 3 relevant variables in an array
// Return -1 = error
// Return 0 = success
int parseWineData(float wine_data[178][3]) {
	
	// Variables
	FILE* file_pointer;
	char line_buffer[255];
	unsigned int nth_number = 1; // The 'n-th' number read in the line
	const char delimiter[2] = ",";
	int number;
	char* token_value;
	int wine_data_index = 0;

	// Open file
	file_pointer = fopen("./wine.data", "r");

	// Check if file exists and then parse through character by character
	// Until we hit the end of file update each users info
	// Only the 2nd, 11th, and 12th values are relevant
	// The other values will be ignored
	if (file_pointer) {
		// Line is stored in line_buffer
		// Have to parse through the line to get the three variables
		while (fgets(line_buffer, sizeof(line_buffer), file_pointer)) {
			// Get the first token and then walk through the other tokens (until the 13th token)
			token_value = strtok(line_buffer, delimiter);
			while(token_value != NULL && nth_number != 13) {
				// Store relevant data if not the 3rd token
				if (nth_number == 2) {
					wine_data[wine_data_index][0] = atof(token_value);
				} 
				else if (nth_number == 11) {
					wine_data[wine_data_index][1] = atof(token_value);
				}
		       		else if (nth_number == 12) {
					wine_data[wine_data_index][2] = atof(token_value);
				} 
				else { // nth_number < 10 and !=2, or error
					// Ignore value
				}
				nth_number++;
				// Continue until hit next delimiter
				token_value = strtok(NULL, delimiter);
			}
			nth_number = 1;
			// Go to the next user
			wine_data_index++;
		}
	}
	else {
		// No file found, print error and close application
		printf("No file ./wine.data found.\n");
		printf("Closing program.\n");
		return -1;
	}

	// Close the file
	fclose(file_pointer);
}

// Pressing escape closes the GUI
void detectKeyboard(unsigned char key, int x, int y) {

        // If escape is pressed, quit the GUI
        if (key == 27) {
                printf("Closing GUI.\n");
                glutLeaveMainLoop();
        }
}

// Sets up OpenGL of scatter plots as well as a keyboard function
void displayGUI(int argc, char **argv) {

	// Initialize GLUT and process user parameters
        glutInit(&argc, argv);

        // Request double buffered true colour window with Z-buffer
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

        // Create window
        glutInitWindowSize(1200, 900);
        glutInitWindowPosition(100, 100);
        glutCreateWindow("ENSC482 Michael Chyziak - Homework 2");

        // Update depth buffer if test passes
        glEnable(GL_DEPTH_TEST);

        // Callback functions
        glutDisplayFunc(drawScreen);
        glutKeyboardFunc(detectKeyboard);

        // Pass control to GLUT for events
        glutMainLoop();
}

// Draws the 3x3 matrix with scatter plot of the 3 wine variables (alcohol, colour intensity, hue)
void drawScreen() {

	// Variables
	int matrix_row_index;
	int matrix_col_index;
	float matrix_row_start = -0.85;
	float matrix_col_start = 0.85;
	float matrix_row_delta = 0.5;
	float matrix_col_delta = 0.5;
	float matrix_row_buffer = 0.1;
	float matrix_col_buffer = 0.1;
	float max_wine_variable_value[3] = {0}; // for 3 variables: alcohol, colour intensity, and hue
	int wine_index;
	int wine_variable_index;
	float relative_scatterplot_x_value;
	float relative_scatterplot_y_value;
	string display_string;

        // Set Background Color to a light grey
        glClearColor(0.4, 0.4, 0.4, 1.0);

        // Clear screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Reset transformations
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

	// Determine maximum and minimum variable values (alcohol, colour intensity, and hue)
	for (wine_index = 0; wine_index < 178; wine_index++) {
		for (wine_variable_index = 0; wine_variable_index < 3; wine_variable_index++) {
			if (wine_data[wine_index][wine_variable_index] > max_wine_variable_value[wine_variable_index]) {
				max_wine_variable_value[wine_variable_index] = wine_data[wine_index][wine_variable_index];
			}
		}
	}

	// 3x3 Matrix of Scatter plot diagrams
	// Providing also axis bars and axis values
	for (matrix_row_index = 0; matrix_row_index < 3; matrix_row_index++) {
		for (matrix_col_index = 0; matrix_col_index < 3; matrix_col_index++) {
			// 3x3 matrix
			glBegin(GL_LINES);
			glColor3f(0, 0, 0);
			glVertex2f(matrix_row_start + (matrix_row_index * (matrix_row_delta + matrix_row_buffer)), 
					matrix_col_start - (matrix_col_index * (matrix_col_delta + matrix_col_buffer)));
			glVertex2f((matrix_row_start + matrix_row_delta) + (matrix_row_index * (matrix_row_delta + matrix_row_buffer)), 
					matrix_col_start - (matrix_col_index * (matrix_col_delta + matrix_col_buffer)));
			glEnd();
			glBegin(GL_LINES);
			glColor3f(0, 0, 0);
			glVertex2f((matrix_row_start + matrix_row_delta) + (matrix_row_index * (matrix_row_delta + matrix_row_buffer)), 
					matrix_col_start - (matrix_col_index * (matrix_col_delta + matrix_col_buffer)));
			glVertex2f((matrix_row_start + matrix_row_delta) + (matrix_row_index * (matrix_row_delta + matrix_row_buffer)), 
					(matrix_col_start - matrix_col_delta) - (matrix_col_index * (matrix_col_delta + matrix_col_buffer)));
			glEnd();
			glBegin(GL_LINES);
			glColor3f(0, 0, 0);
			glVertex2f((matrix_row_start + matrix_row_delta) + (matrix_row_index * (matrix_row_delta + matrix_row_buffer)), 
					(matrix_col_start - matrix_col_delta) - (matrix_col_index * (matrix_col_delta + matrix_col_buffer)));
			glVertex2f(matrix_row_start + (matrix_row_index * (matrix_row_delta + matrix_row_buffer)), 
					(matrix_col_start - matrix_col_delta) - (matrix_col_index * (matrix_col_delta + matrix_col_buffer)));
			glEnd();
			glBegin(GL_LINES);
			glColor3f(0, 0, 0);
			glVertex2f(matrix_row_start + (matrix_row_index * (matrix_row_delta + matrix_row_buffer)), 
					(matrix_col_start - matrix_col_delta) - (matrix_col_index * (matrix_col_delta + matrix_col_buffer)));
			glVertex2f(matrix_row_start + (matrix_row_index * (matrix_row_delta + matrix_row_buffer)), 
					matrix_col_start - (matrix_col_index * (matrix_col_delta + matrix_col_buffer)));
			glEnd();
			// Axis bars and values
			// TODO
		}
	}

	// Draw each scatter plot diagram (bar graph for main diagonal)
	// Each data point is represented by an 'x')
	for (matrix_row_index = 0; matrix_row_index < 3; matrix_row_index++) {
		for (matrix_col_index = 0; matrix_col_index < 3; matrix_col_index++) {
			if (matrix_row_index != matrix_col_index) {
				for (wine_index = 0; wine_index < 178; wine_index++) {
					// Find the points relative value in the scatter plot (between 0 and x and y delta's)
					relative_scatterplot_x_value = wine_data[wine_index][matrix_row_index] / max_wine_variable_value[matrix_row_index] * matrix_row_delta;
					relative_scatterplot_y_value = wine_data[wine_index][matrix_col_index] / max_wine_variable_value[matrix_col_index] * matrix_col_delta;
					// Draw 'x'
					glBegin(GL_LINES);
					glColor3f(0, 0, 1);
					glVertex2f(matrix_row_start + (matrix_row_index * (matrix_row_delta + matrix_row_buffer)) + relative_scatterplot_x_value - 0.01f, 
							matrix_col_start - (matrix_col_index * (matrix_col_delta + matrix_col_buffer)) - relative_scatterplot_y_value + 0.01f) ;
					glVertex2f(matrix_row_start + (matrix_row_index * (matrix_row_delta + matrix_row_buffer)) + relative_scatterplot_x_value + 0.01f, 
							matrix_col_start - (matrix_col_index * (matrix_col_delta + matrix_col_buffer)) - relative_scatterplot_y_value - 0.01f) ;
					glEnd();
					glBegin(GL_LINES);
					glColor3f(0, 0, 1);
					glVertex2f(matrix_row_start + (matrix_row_index * (matrix_row_delta + matrix_row_buffer)) + relative_scatterplot_x_value - 0.01f, 
							matrix_col_start - (matrix_col_index * (matrix_col_delta + matrix_col_buffer)) - relative_scatterplot_y_value - 0.01f) ;
					glVertex2f(matrix_row_start + (matrix_row_index * (matrix_row_delta + matrix_row_buffer)) + relative_scatterplot_x_value + 0.01f, 
							matrix_col_start - (matrix_col_index * (matrix_col_delta + matrix_col_buffer)) - relative_scatterplot_y_value + 0.01f) ;
					glEnd();
				}	
			}
			else {
				// Display variable
				if (matrix_row_index == 0 && matrix_col_index == 0) {
					display_string = "Wine Alcohol";
				}
				else if (matrix_row_index == 1 && matrix_col_index == 1) {
					display_string = "Wine Colour Intensity";
				}
				else if (matrix_row_index == 2 && matrix_col_index == 2) {
					display_string = "Wine Hue";
				}
				else {
					// ERROR: should not reach here
				}
				displayString(matrix_row_start + (matrix_row_index * (matrix_row_delta + matrix_row_buffer)) + 0.1f,
						matrix_col_start - (matrix_col_index * (matrix_col_delta + matrix_col_buffer)) - (matrix_col_delta / 2), display_string);
			}
		}
	}
	
        // Swap to buffer
        glFlush();
        glutSwapBuffers();
}

// Displays the input string on the screen at co-ordinates x and y
// Display is done in bitmap 18 font helvetica, black
void displayString(float x, float y, string input) {
	
	// Cast string to specific format for glutBitmapString function
	const unsigned char* input_cast = reinterpret_cast<const unsigned char*>
		(input.c_str());

	// Display black string on screen
	glColor3f(0, 0, 0);
	glRasterPos2f(x, y);
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, input_cast);
}
