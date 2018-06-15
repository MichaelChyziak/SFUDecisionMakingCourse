// Programmer: Michael Chyziak
// Date: Wednesday June 5, 2018
//
// Solves a 2x2 matrix using a mixed nash equilibrium algorithm.
// The matrix can be changed for any other matrix that has a mixed nash equilibrium.
// Changing the matrix to a non-mixed nash equilibrium has unknown results.
// The matrix is shown on the screen and text is shown as well.
// The text explains the outcome of how each player should play and their payoff
// Also tells the user how to quit the program
//
// g++ mixed_nash.cc -lm -lglut -lGL -lGLU -o mixed_nash

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
void mixedNash(int argc, char **argv);
void detectKeyboard(unsigned char key, int x, int y);
void drawScreen();
void displayString(float x, float y, string input);
void solveMixedNash();

// Payoff Matrix (dimensions are: row, column, cell)
int payoff_matrix[2][2][2] = {{{2, -3}, {1, 2} }, {{1, 1}, {4, -1}}};

// Solution by hand
/*
                    Player 2
Player 1      Option 1     Option 2
Option 1        2,-3          1, 2
Option 2        1, 1          4,-1

Let P be probability Player 2 chooses Option 1, (1-P) be chooses Option 2
P(P1_O1) + (1-P)(P1_O1)
P(P1_O2) + (1-P)(P1_O2)
P(P1_O1) + (1-P)(P1_O1) = P(P1_O2) + (1-P)(P1_O2)
P(2) + (1-P)(1) = P(1) + (1-P)(4)
2P + 1 - P = P + 4 - 4P
P + 1 = -3P + 4
4P = 3
P = 3/4
Player 1 payout:
(3/4)(2) + (1/4)(1) = 1.5 + 0.25 = 1.75

Let P be probability Player 1 chooses Option 1, (1-P) be chooses Option 2
P(P2_O1) + (1-P)(P2_O1)
P(P2_O2) + (1-P)(P2_O2)
P(P2_O1) + (1-P)(P2_O1) = P(P2_O2) + (1-P)(P2_O2)
P(-3) + (1-P)(1) = P(2) + (1-P)(-1)
-3P + 1 - P = 2P - 1 + P
-4P + 1 = 3P - 1
-7P = -2
P = 2/7
Player 2 payout:
(2/7)(-3) + (5/7)(1) = -6/7 + 5/7 = -1/7 = -0.142857142
*/


int main(int argc, char **argv) {

	// Variables
	string user_input;
	bool run_program = true;

	// Program explanation/initialization
	printf("Press *Esc* button or close the GUI to exit the program.\n");
	
	// Run GUI
	mixedNash(argc, argv);
}

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
        glutCreateWindow("ENSC482 Michael Chyziak - Homework 7");

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
