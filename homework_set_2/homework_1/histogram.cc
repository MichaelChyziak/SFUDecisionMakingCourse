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
// TODO
int createHistogram(unsigned int histogram[52]);

int main(int argc, char **argv) {

	// Variables
	unsigned int histogram[52] = {0}; // Each array index is equal to a cards final value
	int status;

	// Program explanation/initialization
	// TODO (explain how cards are stored)
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
	// TODO REMOVE JUSDT DEBUG FOR NOW
	int i;
	for(i=0;i<52;i++){
		printf("histogram[%d] = %d\n", i+1, histogram[i]);
	}	

	// Evaluate mean
	// TODO
	
	// Evaluate variance
	// TODO
	
	// Run GUI????? TODO
}

// TODO
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


//TODO EVERYTHING BELOW HERE HAS NOT BEEN TOUCHED YET, SHOULD PROBABLY BE REMOVED????????
//
//
//
//
//
/*
// Draws and solves the matrix using mixed Nash equilibrium
// Also adds text to display the results
// Sets up OpenGL
void mixedNash(int argc, char **argv) {

	// Initialize GLUT and process user parameters
        glutInit(&argc, argv);

        // Request double buffered true colour window with Z-buffer
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

        // Create window
        glutInitWindowSize(800, 600);
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

// Pressing escape closes the GUI
void detectKeyboard(unsigned char key, int x, int y) {

        // If escape is pressed, quit the GUI
        if (key == 27) {
                printf("Closing GUI.\n");
                glutLeaveMainLoop();
        }
}

// Draws and solves the matrix using mixed Nash equilibrium
// Also adds text to display the results
void drawScreen() {

        // Set Background Color to a light grey
        glClearColor(0.4, 0.4, 0.4, 1.0);

        // Clear screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Reset transformations
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

	// Payoff Matrix
	// Grid Lines
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
	displayString(-0.1f, 0.7f, to_string(payoff_matrix[0][0][0]) + "," + 
			to_string(payoff_matrix[0][0][1]));
	displayString(0.2, 0.7f, to_string(payoff_matrix[0][1][0]) + "," + 
			to_string(payoff_matrix[0][1][1]));
	displayString(-0.1f, 0.6f, to_string(payoff_matrix[1][0][0]) + "," + 
			to_string(payoff_matrix[1][0][1]));
	displayString(0.2, 0.6f, to_string(payoff_matrix[1][1][0]) + "," + 
			to_string(payoff_matrix[1][1][1]));
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

	solveMixedNash();

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

// Solves the mixed nash equilibrium using equations
// Equations can be used any arbitrary matrix that has a mixed nash equilibrium
// Also displays text on the GUI screen of the mixed nash outcome and payoffs
void solveMixedNash() {

	// Variables
	// Format: PlayerX_OptionX_PlayerY_OptionY gives the value in that cell to Player X
	// eg. P1_O1_P2_O2 = Player 1 with Option 1 and Player 2 with Option 2, value for P1
	float P1_O1_P2_O1 = payoff_matrix[0][0][0];
	float P1_O1_P2_O2 = payoff_matrix[0][1][0];
	float P1_O2_P2_O1 = payoff_matrix[1][0][0];
	float P1_O2_P2_O2 = payoff_matrix[1][1][0];
	float P2_O1_P1_O1 = payoff_matrix[0][0][1];
	float P2_O1_P1_O2 = payoff_matrix[1][0][1];
	float P2_O2_P1_O1 = payoff_matrix[0][1][1];
	float P2_O2_P1_O2 = payoff_matrix[1][1][1];
	float prob_P1;
	float prob_P2;
	float payoff_P1;
	float payoff_P2;

	// Solve Player 1 and Player 2 Probabilities
	prob_P1 = (P2_O2_P1_O2 - P2_O1_P1_O2) / 
		(P2_O1_P1_O1 - P2_O1_P1_O2 - P2_O2_P1_O1 + P2_O2_P1_O2);
	prob_P2 = (P1_O2_P2_O2 - P1_O1_P2_O2) / 
		(P1_O1_P2_O1 - P1_O1_P2_O2 - P1_O2_P2_O1 + P1_O2_P2_O2);

	// Solve Player 1 and Player 2 Payoffs
	payoff_P1 = (prob_P2 * P1_O1_P2_O1) + ((1.0f - prob_P2) * P1_O1_P2_O2);
	payoff_P2 = (prob_P1 * P2_O1_P1_O1) + ((1.0f - prob_P1) * P2_O1_P1_O2);

	// Mixed Nash P1 and P2 probabilities text
	displayString(-0.5f, 0.5f, "The best Nash equilibrium in mixed strategies is:");
	displayString(-0.8f, 0.4f, "Player 1 plays a mixed strategy of (Option 1, Option 2)"
			" = (" + to_string(prob_P1) + ", " + to_string(1.0f-prob_P1) + ")");
	displayString(-0.8f, 0.3f, "Player 2 plays a mixed strategy of (Option 1, Option 2)"
			" = (" + to_string(prob_P2) + ", " + to_string(1.0f-prob_P2) + ")");

	// Payout Text
	displayString(-0.2f, 0.1f, "The payoff to each player is:");
	displayString(-0.4f, 0, "Player 1 has an expected payoff = " + 
			to_string(payoff_P1));
	displayString(-0.4f, -0.1f, "Player 2 has an expected payoff = " + 
			to_string(payoff_P2));
	
	// Quit Program Text
	displayString(-0.5f, -0.5f, "Press *Esc* button or close the GUI to exit the program");
}
*/
