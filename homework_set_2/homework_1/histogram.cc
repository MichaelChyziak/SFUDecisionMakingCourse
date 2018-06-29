// Programmer: Michael Chyziak
// Date: Wednesday June 14, 2018
//
// TODO: program description
// TODO: required supplementary files
// TODO: got data set from here: 
// 	http://archive.ics.uci.edu/ml/machine-learning-databases/poker/poker-hand-testing.data
// 
// g++ histogram.cc -lm -lglut -lGL -lGLU -o histogram

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

// TODO remove Homework description below
/*
Problem 1 (30) – By referring to any known public dataset (some examples are listed below but you are free to reference to any other known sources), write a program in Visual C++ that takes the dataset as an input file and computes the histogram of the data. Visualize the histogram data using OpenGL. Compute the mean and the variance of the distribution and overlay and visualize the expression for its univariate Gaussian distribution. Given two such histogram, compute the Bhattacharyya distance between them. Include the selected input dataset as a part of your submission. Include a short write-up discussing the results.
*/

using namespace std;

// Function Declarations
int createHistogram(unsigned int histogram[52]);
float calculateMean(unsigned int histogram[52]);
float calculateVariance(unsigned int histogram[52], float mean);
void detectKeyboard(unsigned char key, int x, int y);
void displayGUI(int argc, char **argv);
void drawScreen();
void displayString(float x, float y, string input);

int main(int argc, char **argv) {

	// Variables
	unsigned int histogram[52] = {0}; // Each array index is equal to a cards final value
	int status;
	float mean;
	float variance;

	// Program explanation/initialization
	// TODO (explain how cards are stored in printf statements)
	// Cards stored as tuples (suit, value)
	// Suits are:
	// 1 = hearts
	// 2 = spades
	// 3 = diamonds
	// 4 = clubs
	// Values are:
	// 1 = Ace
	// 2-10: face value cards
	// 11 = Jack
	// 12 = Queen
	// 13 = King
	// Going to parse all 100,000 hands which have 5 cards each
	// Going to show a histogram of 500,000 cards in a deck, drawn randomly 5 at a time
	// The value of a card will be its (value + (13 * (suit-1))
	// Thus the final value of cards will range from 1 to 52

	// Parses input from file and stores into histogram array
	// If cannot read from file it ends the program
	status = createHistogram(histogram);
	if (status == -1) {
		return status;		
	}
	// TODO REMOVE: JUST DEBUG FOR NOW
	int i;
	for(i=0;i<52;i++){
		printf("histogram[%d] = %d\n", i+1, histogram[i]);
	}	

	// Evaluate mean
	mean = calculateMean(histogram);

	// Evaluate variance
	variance = calculateVariance(histogram, mean);
	
	// TODO: Bhattacharyya with 2 histograms??? (where does 2nd come from?)
	
	// Run GUI
	displayGUI(argc, argv);
}

// Creates a histogram where the value of each card is put into the respective array index
// value of card = array - 1
// number at that index = number of that card type from input file
// Return -1 = error
// Return 0 = success
int createHistogram(unsigned int histogram[52]) {
	
	// Variables
	FILE* file_pointer;
	char line_buffer[255];
	unsigned int nth_number = 1; // The 'n-th' number read in the line
	const char delimiter[2] = ",";
	int number;
	char* number_token;
	int suit_value;

	// Open file
	file_pointer = fopen("./poker-hand-testing.data", "r");

	// Check if file exists and then parse through character by character
	// Until we hit the end of file add each card value to the histogram array
	// Only the first 10 numbers of each line correspond to a card value (suit, value)
	// The 11th value should be ignored as it just sums up the 5 cards score/ranking
	if (file_pointer) {
		// Line is stored in line_buffer
		// Have to parse through the line to get the first 10 numbers
		while (fgets(line_buffer, sizeof(line_buffer), file_pointer)) {
			// Get the first token and then walk through the other tokens
			number_token = strtok(line_buffer, delimiter);
			while(number_token != NULL && nth_number != 11) {
				// Store number if suit to later multiply with card value
				// Otherwise multiply value with suit and put in histogram
				if (nth_number % 2 == 1) {
					// change suits from 1-4 to 0-3
					suit_value = atoi(number_token) - 1;
				} else {
					histogram[((suit_value*13) + atoi(number_token)) - 1] += 1;
				}
				nth_number++;
				// Continue until hit next delimiter
				number_token = strtok(NULL, delimiter);
			}
			nth_number = 1;
		}
	}
	else {
		// No file found, print error and close application
		printf("No file ./poker-hand-testing.data found.\n");
		printf("Closing program.\n");
		return -1;
	}

	// Close the file
	fclose(file_pointer);
}

// Calculates the mean value from a histogram and returns the mean
float calculateMean(unsigned int histogram[52]) {
        
	// Variables
	int histogram_index;
	int number_of_values = 0;
	int sum_value = 0;
	float mean_value;

	// Calculates the sum of all of the values and the number of values taken
	for (histogram_index = 0; histogram_index < 52; histogram_index++) {
		sum_value += (histogram[histogram_index] * (histogram_index + 1));
		number_of_values += histogram[histogram_index];                
	}
        
	// Calculates and returns the mean
	mean_value = ((float) sum_value) / ((float) number_of_values);
	return mean_value;
}

// Calculates and returns the variance when the histogram and mean is provided
float calculateVariance(unsigned int histogram[52], float mean) {

	// Variables
	int histogram_index;
	int number_of_values = 0;
	float summation_value = 0;
	float variance;

	// Calculates the number of values in the histogram
	for (histogram_index = 0; histogram_index < 52; histogram_index++) {
		number_of_values += histogram[histogram_index];                
	}

	// Calculates the summation of (value - mean)^2
	for (histogram_index = 0; histogram_index < 52; histogram_index++) {
		summation_value = (((histogram_index + 1) - mean) * ((histogram_index + 1) - mean));
		summation_value = summation_value * histogram[histogram_index];
	}

	// Calculates the variance and returns the value
	variance = 1 / (number_of_values - 1);
	variance = variance * summation_value;
	return variance;
}

// Pressing escape closes the GUI
void detectKeyboard(unsigned char key, int x, int y) {

        // If escape is pressed, quit the GUI
        if (key == 27) {
                printf("Closing GUI.\n");
                glutLeaveMainLoop();
        }
}

// Draws the histogram 
// shows the univariate Gaussian distribution with the mean and variance already calculated
// TODO Bayychetta? or w/e the spelling is with 2 histograms?????
// Sets up OpenGL
void displayGUI(int argc, char **argv) {

	// Initialize GLUT and process user parameters
        glutInit(&argc, argv);

        // Request double buffered true colour window with Z-buffer
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

        // Create window
        glutInitWindowSize(1200, 900);
        glutInitWindowPosition(100, 100);
        glutCreateWindow("ENSC482 Michael Chyziak - Homework 1");

        // Update depth buffer if test passes
        glEnable(GL_DEPTH_TEST);

        // Callback functions
        glutDisplayFunc(drawScreen);
        glutKeyboardFunc(detectKeyboard);

        // Pass control to GLUT for events
        glutMainLoop();
}

// Draws the histogram and the overlayed invariante Gaussian distribution
void drawScreen() {

        // Set Background Color to a light grey
        glClearColor(0.4, 0.4, 0.4, 1.0);

        // Clear screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Reset transformations
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

	// Draw histogram
	// TODO
	
	// Draw univariate Gaussian distribution
	// TODO
	
	// Display mean and variance
	// TODO
	
	// TODO REMOVE START
	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	glVertex2f(-0.55f, 0.975f);
	glVertex2f(0.4f, 0.975f);
	glEnd();
	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	glVertex2f(-0.55f, 0.875f);
	glVertex2f(0.4f, 0.875f);
	glEnd();
	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	glVertex2f(-0.55f, 0.775);
	glVertex2f(0.4f, 0.775f);
	glEnd();
	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	glVertex2f(-0.55f, 0.675f);
	glVertex2f(0.4f, 0.675f);
	glEnd();
	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	glVertex2f(-0.55f, 0.575f);
	glVertex2f(0.4f, 0.575f);
	glEnd();
	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	glVertex2f(-0.55f, 0.575f);
	glVertex2f(-0.55f, 0.975f);
	glEnd();
	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	glVertex2f(-0.25f, 0.575f);
	glVertex2f(-0.25f, 0.975f);
	glEnd();
	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	glVertex2f(0.1f, 0.575f);
	glVertex2f(0.1f, 0.875f);
	glEnd();
	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	glVertex2f(0.4f, 0.575f);
	glVertex2f(0.4f, 0.975f);
	glEnd();
	// Text/Values
	displayString(-0.5f, 0.8f, "Player 1");
	displayString(0, 0.9f, "Player 2");
	displayString(-0.5f, 0.7f, "Option 1");
	displayString(-0.5f, 0.6f, "Option 2");
	displayString(-0.15f, 0.8f, "Option 1");
	displayString(0.15f, 0.8f, "Option 2");
	// Grid Box Colours
	glBegin(GL_POLYGON);
	glColor3f(1, 0.271f, 0);
	glVertex2f(-0.55, 0.975);
	glVertex2f(0.4, 0.975);
	glVertex2f(0.4, 0.775);
	glVertex2f(-0.55, 0.775);
	glEnd();
	glBegin(GL_POLYGON);
	glColor3f(1, 0.271f, 0);
	glVertex2f(-0.55, 0.775);
	glVertex2f(-0.25, 0.775);
	glVertex2f(-0.25, 0.575);
	glVertex2f(-0.55, 0.575);
	glEnd();
	glBegin(GL_POLYGON);
	glColor3f(1, 0.627f, 0.478f);
	glVertex2f(-0.25, 0.775);
	glVertex2f(0.4, 0.775);
	glVertex2f(0.4, 0.575);
	glVertex2f(-0.25, 0.575);
	glEnd();
	// TODO REMOVE END

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
