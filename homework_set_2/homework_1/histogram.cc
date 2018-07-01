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
int createHistogram(unsigned int histogram[52]);
float calculateMean(unsigned int histogram[52]);
float calculateVariance(unsigned int histogram[52], float mean);
void detectKeyboard(unsigned char key, int x, int y);
void displayGUI(int argc, char **argv);
void drawScreen();
void displayString(float x, float y, string input);
void calculateGaussian(float gaussian[52], float mean, float variance);
void normalizeHistogram(unsigned int histogram[52], float global_normalized_histogram[52]);
float calculateBhattacharyya(float global_normalized_histogram[52], float global_gaussian[52]);

// Global Variables
unsigned int global_histogram[52] = {0}; // Each array index is equal to a cards final value
float global_normalized_histogram[52] = {0}; // Each array index is equal to a cards final value
float  global_gaussian[52] = {0.0f}; // Each array index is equal to a cards final value
float global_mean;
float global_variance;
float bhattacharyya_distance;

// Global Constants
const double PI = 3.141592653589793;

int main(int argc, char **argv) {

	// Variables
	int status;

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
	status = createHistogram(global_histogram);
	if (status == -1) {
		return status;		
	}

	// Normalize histogram
	normalizeHistogram(global_histogram, global_normalized_histogram);

	// Evaluate mean
	global_mean = calculateMean(global_histogram);

	// Evaluate variance
	global_variance = calculateVariance(global_histogram, global_mean);
	
	// evaluate univariate gaussian distribution
	calculateGaussian(global_gaussian, global_mean, global_variance);

	// Bhattacharyya distance
	bhattacharyya_distance = calculateBhattacharyya(global_normalized_histogram, global_gaussian);
	
	// Run GUI
	displayGUI(argc, argv);
}

// Calculate the Bhattacharyya distance between two normalized histograms
float calculateBhattacharyya(float global_normalized_histogram[52], float global_gaussian[52]) {
	
	// Variables
	int index;
	int bhattacharyya_d = 0;

	// Calculate d
	for (index = 0; index < 52; index++) {
		bhattacharyya_d += sqrt(global_normalized_histogram[index] * global_gaussian[index]);
	}

	// Calculate and return D
	return log(-1 * bhattacharyya_d);
}

// Normalizes the histogram data and stores in global normalized histogram variable
void normalizeHistogram(unsigned int histogram[52], float global_normalized_histogram[52]) {
	
	// Variables
	unsigned int total_count = 0;
	int histogram_index;

	// Count number of poker hands
	for (histogram_index = 0; histogram_index < 52; histogram_index++) {
		total_count += histogram[histogram_index];
	}

	// Normalize histogram
	for (histogram_index = 0; histogram_index < 52; histogram_index++) {
		global_normalized_histogram[histogram_index] = (float)histogram[histogram_index] / (float)total_count;
	}
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
	float temp_summation_value = 0;
	float variance;

	// Calculates the number of values in the histogram
	for (histogram_index = 0; histogram_index < 52; histogram_index++) {
		number_of_values += histogram[histogram_index];                
	}

	// Calculates the summation of (value - mean)^2
	for (histogram_index = 0; histogram_index < 52; histogram_index++) {
		temp_summation_value = (((histogram_index + 1) - mean) * ((histogram_index + 1) - mean));
		temp_summation_value = temp_summation_value * histogram[histogram_index];
		summation_value += temp_summation_value;
	}

	// Calculates the variance and returns the value
	variance = 1.0 / (number_of_values - 1);
	variance = variance * summation_value;
	return variance;
}

// Calculates the univariate gaussian distribution
// mean and variance are passed in as parameters
void calculateGaussian(float gaussian[52], float mean, float variance) {
	
	// Variables
	int gaussian_index;
	float temp_gaussian;

	// Calculates the value of the gaussian at each point on the histogram
	for (gaussian_index = 0; gaussian_index < 52; gaussian_index++) {
		temp_gaussian = pow(gaussian_index - mean, 2) / (-2 * variance);
		gaussian[gaussian_index] = 1.0f / sqrt(2 * PI * variance);
		gaussian[gaussian_index] = gaussian[gaussian_index] * exp(temp_gaussian);
	}
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
// Also shows the Bhattacharyya distance between the two normalized histograms
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
        //glEnable(GL_DEPTH_TEST); // Means first drawn = background, latest drawn = foreground

        // Callback functions
        glutDisplayFunc(drawScreen);
        glutKeyboardFunc(detectKeyboard);

        // Pass control to GLUT for events
        glutMainLoop();
}

// Draws the histogram and the overlayed univariate Gaussian distribution
void drawScreen() {

	// Variables
	int histogram_index;
	float graph_max = 0;
	int index_bar_x;
	int index_bar_y;
	float bar_x_delta = 1.6f/52.0f;
	float bar_y_delta = 1.6f/10.0f;

        // Set Background Color to a light grey
        glClearColor(0.4, 0.4, 0.4, 1.0);

        // Clear screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Reset transformations
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

	// Find max value of graph (original and gaussian histograms)
	for (histogram_index = 0; histogram_index < 52; histogram_index++) {
		if (global_normalized_histogram[histogram_index] > graph_max) {
			graph_max = global_normalized_histogram[histogram_index];
		}
		if (global_gaussian[histogram_index] > graph_max) {
			graph_max = global_gaussian[histogram_index];
		}
	}

	// Draw histogram
	// Create Title and display mean and variance
	displayString(-0.6f, 0.9f, "Poker Hands Histogram (Mean = " + to_string(global_mean) + ", Variance = " + to_string(global_variance) + 
			", Bhattacharyya distance = " + to_string(bhattacharyya_distance) + ")");
	// Draw X and Y coordinates
	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	glVertex2f(-0.8f, -0.8f);
	glVertex2f(0.8f, -0.8f);
	glEnd();
	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	glVertex2f(-0.8f, -0.8f);
	glVertex2f(-0.8f, 0.8f);
	glEnd();
	for (index_bar_x = 0; index_bar_x < 53; index_bar_x++) {
		glBegin(GL_LINES);
		glColor3f(0, 0, 0);
		glVertex2f(-0.8f + (index_bar_x * bar_x_delta), -0.8f);
		glVertex2f(-0.8f + (index_bar_x * bar_x_delta), -0.85f);
		glEnd();	
	}
	for (index_bar_y = 0; index_bar_y < 11; index_bar_y++) {
		glBegin(GL_LINES);
		glColor3f(0, 0, 0);
		glVertex2f(-0.8f, -0.8f + (index_bar_y * bar_y_delta));
		glVertex2f(-0.85f, -0.8f + (index_bar_y * bar_y_delta));
		glEnd();	
	}
	// Label X and Y coordinates
	displayString(-0.1f, -0.95f, "Poker Hand");
	displayString(-0.99f, 0.2f, "P");
	displayString(-0.99f, 0.16f, "r");
	displayString(-0.99f, 0.12f, "o");
	displayString(-0.99f, 0.08f, "b");
	displayString(-0.99f, 0.04f, "a");
	displayString(-0.99f, 0.0f, "b");
	displayString(-0.99f, -0.04f, "i");
	displayString(-0.99f, -0.08f, "l");
	displayString(-0.99f, -0.12f, "i");
	displayString(-0.99f, -0.16f, "t");
	displayString(-0.99f, -0.20f, "y");
	for (index_bar_x = 0; index_bar_x < 53; index_bar_x++) {
		if (index_bar_x % 4 == 0) {
			displayString(-0.81f + (index_bar_x * bar_x_delta), -0.88f, to_string(index_bar_x));
		}
	}
	for (index_bar_y = 0; index_bar_y < 11; index_bar_y++) {
		if (index_bar_y % 2 == 0) {
			displayString(-0.95f, -0.79f + (index_bar_y * bar_y_delta), to_string((graph_max / 10.0f * index_bar_y)));
		}
	}
	// Draw bars for data histogram
	for (histogram_index = 0; histogram_index < 52; histogram_index++) {
		glBegin(GL_POLYGON);
		glColor3f(1.0f, 0.271f, 0.0f);
		glVertex2f(-0.795f + (histogram_index * bar_x_delta), -0.8);
		glVertex2f(-0.805f + ((histogram_index+1) * bar_x_delta), -0.8);
		glVertex2f(-0.805f + ((histogram_index+1) * bar_x_delta), -0.8 + (1.6f * global_normalized_histogram[histogram_index] / graph_max));
		glVertex2f(-0.795f + (histogram_index * bar_x_delta), -0.8 + (1.6f * global_normalized_histogram[histogram_index] / graph_max));
		glEnd();
	}
	
	// Draw univariate Gaussian distribution
	for (histogram_index = 0; histogram_index < 52; histogram_index++) {
		glBegin(GL_LINES);
		glColor3f(0, 0, 0);
		glVertex2f(-0.8f + ((histogram_index+1) * bar_x_delta), -0.8 + (1.6f * global_gaussian[histogram_index] / graph_max));
		glVertex2f(-0.8f + (histogram_index * bar_x_delta), -0.8 + (1.6f * global_gaussian[histogram_index] / graph_max));
		glEnd();	
		glEnd();
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
