// Programmer: Michael Chyziak
// Date: Wednesday June 5, 2018
//
// This program shows the user a matrix from the Prisoner's Dilemma Game. 
// The user can either select to determine the best response strategy for each player.
// The matrix can be changed in the source code.
// If the matrix is one where a player doesn't have a best response strategy the program will notify the user.
//
// g++ prisoners.cc -lm -lglut -lGL -lGLU -o prisoners

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
void bestResponse();

// Struct for the number of years per matrix value for both players
// p1 = player1, p2 = player2
struct prisonYears {
	int p1;
	int p2;
};

// Decision Type
enum decisionType {Confess, NoConfess, None};

// Payoff Matrix
prisonYears payoff_matrix[2][2];


int main(int argc, char **argv) {

	// Variables
	string user_input;
	bool run_program = true;

	// Player decisions
	// P1 = player 1, P2 = player2
	// C = confess, NC = not confess
	// ex. P1C_P2C = player 1 confess and player 2 confess
	prisonYears P1C_P2C;
	prisonYears P1C_P2NC;
	prisonYears P1NC_P2C;
	prisonYears P1NC_P2NC;

	// Payoff Matrix assignment
	P1C_P2C.p1 = -5;
	P1C_P2C.p2 = -5;
	P1C_P2NC.p1 = -1;
	P1C_P2NC.p2 = -10;
	P1NC_P2C.p1 = -10;
	P1NC_P2C.p2 = -1;
	P1NC_P2NC.p1 = -2;
	P1NC_P2NC.p2 = -2;
	payoff_matrix[0][0] = P1NC_P2NC;
	payoff_matrix[1][0] = P1C_P2NC;
	payoff_matrix[0][1] = P1NC_P2C;
	payoff_matrix[1][1] = P1C_P2C;

	// Run program while running condition is met
	while(run_program == true) {

		// Program explanation/initialization
		printf("This program tries to determine the best response strategy for the Prisoner's Dilemma Game.\n");
		printf("Values are shown as negative to denote the number of years taken off of their life by going to prison.\n");
		printf("The payoff matrix looks as follows (C = Confess, NC = Not Confess):\n");
		printf("|          |        Player 2       |\n");
		printf("| Player 1 |    NC     |     C     |\n");
		printf("|----------|-----------|-----------|\n");
		printf("|      NC  |  %3d,%3d  |  %3d,%3d  |\n", payoff_matrix[0][0].p1, payoff_matrix[0][0].p2, payoff_matrix[0][1].p1, payoff_matrix[0][1].p2);
		printf("|       C  |  %3d,%3d  |  %3d,%3d  |\n", payoff_matrix[1][0].p1, payoff_matrix[1][0].p2, payoff_matrix[1][1].p1, payoff_matrix[1][1].p2);

		printf("Enter \'1\' to run the best response strategy.\n");
		printf("Enter \'2\' to close the program.\n");

		// Get user input
		printf("User Selection: ");
		cin >> user_input;

		// Check if input is valid
		if (user_input.length() == 1) {
			switch (user_input.at(0)) {
				case '1':
					// best response strategy
					bestResponse();
					printf("\n");
					break;
				case '2':
					// Close program
					printf("Closing program.\n");
					run_program = false;
					break;
				default:
					// Invalid input
					printf("Error: invalid user input. Try "
						"again.\n\n");
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

void bestResponse() {
	
	// Variables	
	int P1NC_if_P2C = payoff_matrix[0][1].p1;
	int P1C_if_P2C = payoff_matrix[1][1].p1;
	int P1NC_if_P2NC = payoff_matrix[0][0].p1;
	int P1C_if_P2NC = payoff_matrix[1][0].p1;
	int P2NC_if_P1C = payoff_matrix[1][0].p2;
	int P2C_if_P1C = payoff_matrix[1][1].p2;
	int P2NC_if_P1NC = payoff_matrix[0][0].p2;
	int P2C_if_P1NC = payoff_matrix[0][1].p2;
	decisionType decision_p1;
	decisionType decision_p2;

	// Player 1 best response strategy
	// If player 2 C, if NC better than C
	if (P1NC_if_P2C > P1C_if_P2C) {
		// If player 2 NC, if NC better than C
		if (P1NC_if_P2NC > P1C_if_P2NC) {
			decision_p1 = NoConfess;
		}
		// If player 2 NC, if C better than NC
		else {
			decision_p1 = None;
		}
	}
	// If player 2 C, if C better than NC
	else {
		// If player 2 NC, if C better than NC
		if (P1C_if_P2NC > P1NC_if_P2NC) {
			decision_p1 = Confess;
		}
		// If player 2 NC, if NC better than C
		else {
			decision_p1 = None;
		}
	}
	

	// Player 2 best response strategy
	// If player 1 C, if NC better than C
	if (P2NC_if_P1C > P2C_if_P1C) {
		// If player 1 NC, if NC better than C
		if (P2NC_if_P1NC > P2C_if_P1NC) {
			decision_p2 = NoConfess;
		}
		// If player 1 NC, if C better than NC
		else {
			decision_p2 = None;
		}
	}
	// If player 1 C, if C better than NC
	else {
		// If player 1 NC, if C better than NC
		if (P2C_if_P1NC > P2NC_if_P1NC) {
			decision_p2 = Confess;
		}
		// If player 1 NC, if NC better than C
		else {
			decision_p2 = None;
		}
	}

	// Output best response strategy 
	printf("The best response strategy for this matrix is:\n");
	switch (decision_p1) {
		case Confess:
			printf("Player 1: Confess\n");
			break;
		case NoConfess:
			printf("Player 1: No Confession\n");
			break;
		case None:
			printf("Player 1: No best decision\n");
			break;
		default:
			printf("Error\n");
			break;
	}
	switch (decision_p2) {
		case Confess:
			printf("Player 2: Confess\n");
			break;
		case NoConfess:
			printf("Player 2: No Confession\n");
			break;
		case None:
			printf("Player 2: No best decision\n");
			break;
		default:
			printf("Error\n");
			break;
	}

	// Check if a player has no best strategy
	if (decision_p1 == None || decision_p2 == None) {
		printf("Since a player had no best decision, it is recommended to try another strategy other than the best response strategy.\n");
	}
}
