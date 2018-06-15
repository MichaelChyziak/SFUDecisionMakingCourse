// Programmer: Michael Chyziak
// Date: Wednesday June 2, 2018
//
// A program which allows the user to play a game of roulette.
// The user can select to either play, view the decision matrix, or view the decision tree.
// While viewing the decision tree the user is also able to play.
// The matrix and decision tree will allow the user to know their potential outcomes.
// There are 38 values the user can choose from (0-37 and 00).
// They can bet between values of 1-100.
// The default bet value is set to 1.
// For a win the user gets 35 their bet + their bet.
// For a lose the user gets nothing.
// The OpenGL GUI is idle/frozen while the user is selecting their bet/guess.
//
// g++ roulette.cc -lm -lglut -lGL -lGLU -o roulette

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
int inputToInt(string user_input);
void playRoulette();
void showMatrix();
void showTree(int argc, char** argv);
void updateDisplay(int timer_id);
void idleMain();
void displayString(float x, float y, string input);

int global_bet_amount = 1;

int main(int argc, char **argv) {

	// Variables
	string user_input;
	bool run_program = true;

	// Program explanation
	printf("A roulette wheel has 38 different values, numbers 0-36 and 00.\n");
	printf("The user can only bet dollar amounts between 1 and 100 (integer values).\n");
	printf("If the user wins they will get 35 times their bet + their original bet.\n");
	printf("If the user losses they will get nothing.\n");
	printf("The user can view a decision matrix to understand all of their decisions.\n");
	printf("They can also view a decision tree in OpenGL.\n");
	printf("Good Luck.\n\n");

	// Run program while running condition is met
	while(run_program == true) {

		// Program explanation/initialization
		printf("Enter \'1\' to play roulette.\n");
		printf("Enter \'2\' to view the decision matrix.\n");
		printf("Enter \'3\' to view the decision tree in OpenGL.\n");
		printf("Enter \'4\' to close the program.\n");
	
		// Get user input
		printf("User Selection: ");
		cin >> user_input;

		// Check if input is valid
		if (user_input.length() == 1) {
			switch (user_input.at(0)) {
				case '1':
					// Play Roulette
					playRoulette();
					break;
				case '2':
					// Decision matrix
					showMatrix();
					break;
				case '3':
					// Decision tree
					showTree(argc, argv);
					break;
				case '4':
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

// Checks if a string can be converted to integer values.
// Only integer values from 0 to 999 are accepted and then returned.
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

// Plays a roulette game
// Lets user bet from 1-100 and pick a guess from 0-37
// If they guess right they get their original bet + 35 times their bet
// If they guess wrong they get nothing
void playRoulette() {

	// Variables	
	bool valid_input = false;
	int user_bet_amount;
	int user_guess_value;
	int game_outcome;
	string user_input;

	// User bet value
	// Check if user input is a number between 1 and 100
	printf("\n");
	while (valid_input == false) {
		printf("Please input the bet amount (1-100):\n");
		cin >> user_input;
		user_bet_amount = inputToInt(user_input);
		if (user_bet_amount >= 1 && user_bet_amount <= 100) {
			valid_input = true;
			global_bet_amount = user_bet_amount;
		}
		else {
			// Invalid input
			printf("Error: invalid user input. Try again.\n");
		}
	}

	// Reset input validity
	valid_input = false;

	// User guess value
	// Check if user input is a number between 0 and 37 (37 = 00)
	while (valid_input == false) {
		printf("Please input the guess value 0-37 (37 = 00):\n");
		cin >> user_input;
		user_guess_value = inputToInt(user_input);
		if (user_guess_value >= 0 && user_guess_value <= 37) {
			valid_input = true;
		}
		else {
			// Invalid input
			printf("Error: invalid user input. Try again.\n");
		}
	}

	// Roulette game outcome
	game_outcome = rand() % 38; //provides number between 0 and 37
	printf("\n");
	printf("Roulette outcome is: %d\n", game_outcome);

	// Check if winner
	if (game_outcome == user_guess_value) {
		printf("Congratulations, you won: %d\n", user_bet_amount*35+user_bet_amount);
	}
	else {
		printf("Sorry, better luck next time.\n");
	}
	printf("\n");
}

// Shows the decision matrix for the game of roulette
void showMatrix() {
	
	// Decision Matrix explanation
	printf("\n");
	printf("ROULETTE DECISION MATRIX\n");
	printf("Assume bet amount = X\n");
	printf("All values are in dollars (no cents)\n");
	printf("p = probability\n");

	// Decision Matrix
	printf("|------------|--------------------------------|\n");
	printf("|   Player   |        Roulette Outcome        |\n");
	printf("|   Choice   |  Win(p=1/38)  |  Lose(p=37/38) |\n");
	printf("|------------|---------------|----------------|\n");
	printf("|    Play    |  35X+X = 36X  |       0        |\n");
	printf("| Don't Play |       X       |       X        |\n");
	printf("|------------|---------------|----------------|\n");
	printf("\n");
}

// Sets up the OpenGL GUI for the decision tree and runs the sub-main function
// Allows users to play roulette and see the tree update
void showTree(int argc, char** argv) {

	// Terminal message
	printf("\n");
	printf("OpenGL Decision Tree is now displaying.\n");
	printf("Play a roulette game to see the tree update live.\n");
	printf("\n");

	// Initialize GLUT and process user parameters
	glutInit(&argc, argv);

	// Request double buffered true colour window with Z-buffer
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	// Create window
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("ENSC482 Michael Chyziak - Homework 4");
	
	// Update depth buffer if test passes
        glEnable(GL_DEPTH_TEST);

	// Don't exit when GLUT window closed (uses freeglut.h)
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	// Callback functions
	glutDisplayFunc(drawObjects);
	glutTimerFunc(1, updateDisplay, 0);

	// Pass control to GLUT for events
	glutMainLoop();

	// Message once OpenGL GUI is closed
	printf("\n");
	printf("OpenGL GUI closed.\n");
	printf("\n");
}

// Ask the user for some inputs and then re-updates the decision tree
// While getting input from user the GUI is 'frozen'/idle
void updateDisplay(int timer_id) {

	// Another main which causes the GUI to be idle while running
	idleMain();

	// Redisplay
	glutPostRedisplay();

	// Setup next timer	
	glutTimerFunc(1, updateDisplay, 0);
}

// Draws the decision tree as well as the text for the roulette game
// The amount bet and the outcomes can be changed dynamically
// Everything else is static
void drawObjects() {

	// Set Background Color to a light grey
	glClearColor(0.4, 0.4, 0.4, 1.0);

 	// Clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    	// Reset transformations
	glMatrixMode(GL_MODELVIEW);
    	glLoadIdentity();

	// Decision Tree Title and info text
	displayString(-0.4, 0.9, "Live Updated Roulette Decision Tree");
	
	// Decision Tree
	// Squares
	glBegin(GL_POLYGON);
	glColor3f(0, 0, 1);
	glVertex2f(-0.82, 0.22);
	glVertex2f(-0.78, 0.22);
	glVertex2f(-0.78, 0.18);
	glVertex2f(-0.82, 0.18);
	glEnd();
	glBegin(GL_POLYGON);
	glColor3f(0, 0, 1);
	glVertex2f(-0.22, -0.18);
	glVertex2f(-0.18, -0.18);
	glVertex2f(-0.18, -0.22);
	glVertex2f(-0.22, -0.22);
	glEnd();
	glBegin(GL_POLYGON);
	glColor3f(0, 0, 1);
	glVertex2f(0.38, 0.22);
	glVertex2f(0.42, 0.22);
	glVertex2f(0.42, 0.18);
	glVertex2f(0.38, 0.18);
	glEnd();
	glBegin(GL_POLYGON);
	glColor3f(0, 0, 1);
	glVertex2f(-0.22, 0.62);
	glVertex2f(-0.18, 0.62);
	glVertex2f(-0.18, 0.58);
	glVertex2f(-0.22, 0.58);
	glEnd();
	glBegin(GL_POLYGON);
	glColor3f(0, 0, 1);
	glVertex2f(0.38, -0.58);
	glVertex2f(0.42, -0.58);
	glVertex2f(0.42, -0.62);
	glVertex2f(0.38, -0.62);
	glEnd();
	// Lines
	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	glVertex2f(-0.8, 0.2);
	glVertex2f(-0.2, -0.2);
	glEnd();
	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	glVertex2f(-0.8, 0.2);
	glVertex2f(-0.2, 0.6);
	glEnd();
	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	glVertex2f(-0.2, -0.2);
	glVertex2f(0.4, 0.2);
	glEnd();
	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	glVertex2f(-0.2, -0.2);
	glVertex2f(0.4, -0.6);
	glEnd();
	// Text
	displayString(-0.88, 0.25, "Start");
	displayString(-0.3, 0.65, "Don't Play");
	displayString(-0.5, -0.3, "Play (Bet = " + to_string(global_bet_amount) + ")");
	displayString(0.35, 0.25, "Win");
	displayString(0.35, -0.55, "Lose");
	displayString(0.1, -0.05, "P = 1/38");
	displayString(0.1, -0.4, "1-P = 37/38");
	displayString(-0.15, 0.57, "Outcome = " + to_string(global_bet_amount));
	displayString(0.45, 0.17, "Outcome = " + to_string(global_bet_amount * 36));
	displayString(0.45, -0.63, "Outcome = 0");

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

// Runs an alternate main that causes the OpenGL GUI to IDLE until this function completes
// The user can still play roulette and view the decision matrix
// Once this function completes the decision tree is updated
void idleMain() {

	// Variables
	string user_input;
	bool run_gui = true;

	// Run program while running condition is met
	while(run_gui == true) {

		// Program explanation/initialization
		printf("Enter \'1\' to play roulette.\n");
		printf("Enter \'2\' to view the decision matrix.\n");
		printf("Enter \'3\' to close the decision tree in OpenGL.\n");
	
		// Get user input
		printf("User Selection: ");
		cin >> user_input;

		// Check if input is valid
		if (user_input.length() == 1) {
			switch (user_input.at(0)) {
				case '1':
					// Play Roulette
					playRoulette();
					return;
				case '2':
					// Decision matrix
					showMatrix();
					return;
				case '3':
					// Close Decision tree
					run_gui = false;
					glutLeaveMainLoop();
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
}
