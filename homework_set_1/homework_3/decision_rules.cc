// Programmer: Michael Chyziak
// Date: Wednesday June 2, 2018
//
// A program which determines which act should be chosen in the decision problem with a pre-defined 4x4 matrix. 
// The decision rules that can be selected by the user are as follows: 
// 1) the maximin rule, 
// 2) the maximax rule, 
// 3) the minimax regret rule, 
// 4) the optimism-pessimism rule (alpha > 0.25), 
// 5) and the principle of insufficient reason rule.
//
// g++ decision_rules.cc -lm -lglut -lGL -lGLU -o decision_rules

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
void maximin(int act_matrix[4][4]);
void maximax(int act_matrix[4][4]);
void optimism_pessimism(int act_matrix[4][4]);
void minimax_regret(int act_matrix[4][4]);
void poir(int act_matrix[4][4]);

// Original Matrix
int original_matrix[4][4] = {
	{20, 20, 0, 10}, // a1
	{10, 10, 10, 10}, // a2
	{0, 40, 0, 0}, // a3
	{10, 30, 0, 0}}; //a4

int main(int argc, char **argv) {

	// Variables
	string user_input;
	bool run_program = true;
	int act_index;

	// Run program while running condition is met
	while(run_program == true) {

		// Program explanation/initialization
		printf("Original Matrix: \n");
		for (act_index = 0; act_index < 4; act_index++) {
			printf("| a%d | %2d | %2d | %2d | %2d |\n", act_index+1, original_matrix[act_index][0],
				original_matrix[act_index][1], original_matrix[act_index][2], original_matrix[act_index][3]);
		}
		printf("Select which decision rule to perform on the above original matrix.\n");
		printf("Enter \'1\' to apply the maximin rule.\n");
		printf("Enter \'2\' to apply the maximax rule.\n");
		printf("Enter \'3\' to apply the minimax regret rule.\n");
		printf("Enter \'4\' to apply the optimism-pessimism rule.\n");
		printf("Enter \'5\' to apply the principle of insufficient reason rule.\n");
		printf("Enter \'6\' to close the program.\n");

		// Get user input
		printf("User Selection: ");
		cin >> user_input;

		// Check if input is valid
		if (user_input.length() == 1) {
			switch (user_input.at(0)) {
				case '1':
					// maximin rule
					maximin(original_matrix);
					printf("\n");
					break;
				case '2':
					// maximax rule
					maximax(original_matrix);
					printf("\n");
					break;
				case '3':
					// minimax regret rule
					minimax_regret(original_matrix);
					printf("\n");
					break;
				case '4':
					// optimism-pessimism rule
					optimism_pessimism(original_matrix);
					printf("\n");
					break;
				case '5':
					// principle of insufficient reason (poir) rule
					poir(original_matrix);
					printf("\n");
					break;
				case '6':
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

// The maximin decision rule.
// Takes the minimum of each act and then selects the largest one of those
// No need for leximin rule for this matrix, therefore omitting its implementation
void maximin(int act_matrix[4][4]) {
	
	// Variables
	int act_index;
	int index;
	int matrix_min_val[4] = {100, 100, 100, 100}; // no number is larger than 100, should technically be infinity
	int matrix_max_min_val = -1; // no number is smaller than -1, should technically be -infinity
	int matrix_max_min_act;

	// Find the smallest number in each act
	for (act_index = 0; act_index < 4; act_index++) {
		for (index = 0; index < 4; index++) {
			if (act_matrix[act_index][index] < matrix_min_val[act_index]) {
				matrix_min_val[act_index] = act_matrix[act_index][index];
			}
		}
	}

	// Find the largest value between all of the acts min value
	for (act_index = 0; act_index < 4; act_index++) {
		if (matrix_min_val[act_index] > matrix_max_min_val) {
			matrix_max_min_val = matrix_min_val[act_index];
			matrix_max_min_act = act_index;
		}
	}

	// Notify user of outcome
	printf("The outcome of the maximin rule is:\n");
	printf("Act a%d\n", matrix_max_min_act+1);
}


// The maximax decision rule.
// Takes the max of each act, then select the largest one of those
void maximax(int act_matrix[4][4]) {
	
	// Variables
	int act_index;
	int index;
	int matrix_max_val[4] = {-1, -1, -1, -1}; // no number is smaller than -1, should technically be -infinity
	int matrix_max_max_val = -1; // no number is smaller than -1, should technically be -infinity
	int matrix_max_max_act;

	// Find the largest number in each act
	for (act_index = 0; act_index < 4; act_index++) {
		for (index = 0; index < 4; index++) {
			if (act_matrix[act_index][index] > matrix_max_val[act_index]) {
				matrix_max_val[act_index] = act_matrix[act_index][index];
			}
		}
	}

	// Find the largest value between all of the acts max value
	for (act_index = 0; act_index < 4; act_index++) {
		if (matrix_max_val[act_index] > matrix_max_max_val) {
			matrix_max_max_val = matrix_max_val[act_index];
			matrix_max_max_act = act_index;
		}
	}

	// Notify user of outcome
	printf("The outcome of the maximax rule is:\n");
	printf("Act a%d\n", matrix_max_max_act+1);
}


// The optimism-pessimism decision rule
// Finds the largest and smallest number in each act
// Then iterates through an array of alpha values for the calculates:
// 	alpha * max(act) + (1-alpha) * min(act)
// The act with the highest calculated value is chosen
// This is done for each alpha value (starts from 0.3 and increments by 0.05)
void optimism_pessimism(int act_matrix[4][4]) {
	
	// Variables
	int act_index;
	int index;
	float alpha[15] = {0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1};
	int act_max_val[4] = {-1, -1, -1, -1};
	int act_min_val[4] = {100, 100, 100, 100};
	float act_outcome[4];
	float act_outcome_max;
	int final_act_outcome;

	// Find the largest number in each act
	for (act_index = 0; act_index < 4; act_index++) {
		for (index = 0; index < 4; index++) {
			if (act_matrix[act_index][index] > act_max_val[act_index]) {
				act_max_val[act_index] = act_matrix[act_index][index];
			}
		}
	}
	
	// Find the smallest number in each act
	for (act_index = 0; act_index < 4; act_index++) {
		for (index = 0; index < 4; index++) {
			if (act_matrix[act_index][index] < act_min_val[act_index]) {
				act_min_val[act_index] = act_matrix[act_index][index];
			}
		}
	}

	// Iterate through all different alpha values
	for (index = 0; index < 15; index++) {
		
		// Calculate act outcome values
		for (act_index = 0; act_index < 4; act_index++) {
			act_outcome[act_index] = (alpha[index] * act_max_val[act_index]) + ((1.0f-alpha[index]) * act_min_val[act_index]);
		}

		// Select best act outcome
		act_outcome_max = act_outcome[0];
		final_act_outcome = 0;
		for (act_index = 1; act_index < 4; act_index++) {
			if (act_outcome[act_index] > act_outcome_max) {
				act_outcome_max = act_outcome[act_index];
				final_act_outcome = act_index;
			}
		}

		// Notify user of outcome
		printf("The outcome of the optimism-pessimism rule with alpha = %f is:\n", alpha[index]);
		printf("Act a%d\n", final_act_outcome+1);
	}
}

// The minimax regret decision rule
// Creates a regret table by finding each states max value and subtract it from all other values in that state
// From the regret table finds the max value of each act and chooses the minimum of those values, thus minimizing regret
void minimax_regret(int act_matrix[4][4]) {
	
	// Variables
	int act_index;
	int index;
	int regret_val_max;
	int regret_table[4][4];
	int matrix_max_val[4] = {-1, -1, -1, -1}; // no number is smaller t1han -1, should technically be -infinity
	int matrix_min_val = 100;
	int best_act;

	// Create regret table
	for (index = 0; index < 4; index++) {

		// Find max regret value (best return)
		regret_val_max = -1;
		for (act_index = 0; act_index < 4; act_index++) {
			if (act_matrix[act_index][index] > regret_val_max) {
				regret_val_max = act_matrix[act_index][index];
			}
		}

		// Create regret column from max regret value
		for (act_index = 0; act_index < 4; act_index++) {
			regret_table[act_index][index] = regret_val_max - act_matrix[act_index][index];
		}
	}

	// Find max in regret table for each act
	for (act_index = 0; act_index < 4; act_index++) {
		for (index = 0; index < 4; index++) {
			if (regret_table[act_index][index] > matrix_max_val[act_index]) {
				matrix_max_val[act_index] = regret_table[act_index][index];
			}
		}
	}

	// Find optimal choice from maxes in regret table (optimal = min)
	for (act_index = 0; act_index < 4; act_index++) {
		if (matrix_max_val[act_index] < matrix_min_val) {
			matrix_min_val = matrix_max_val[act_index];
			best_act = act_index;
		}
	}

	// Notify user of outcome
	printf("The outcome of the minimax regret rule is:\n");
	printf("Act a%d\n", best_act+1);
}

// The principle of insufficient reason decision rule.
// Assumes each state is as likely to be probable (in this case 1/4 or 0.25)
// Each act is given a value according to each of its states value times the probability
// The act with the largest value is the most optimal decision
void poir(int act_matrix[4][4]) {
	
	// Variables
	int act_index;
	int index;
	float probability_val = 0.25; //0.25 = 1/4
	int act_val[4] = {0, 0, 0, 0};
	int max_val = -1; // no number is smaller than -1, should technically be -infinity
	int best_act;

	// Calculate the value of each act based on equal probabilities of each state
	for (act_index = 0; act_index < 4; act_index++) {
		for (index = 0; index < 4; index++) {
			act_val[act_index] += (act_matrix[act_index][index] * probability_val);
		}
	}

	// Find the largest value between all of the acts values
	for (act_index = 0; act_index < 4; act_index++) {
		if (act_val[act_index] > max_val) {
			max_val = act_val[act_index];
			best_act = act_index;
		}
	}

	// Notify user of outcome
	printf("The outcome of the principle of insufficient reason rule is:\n");
	printf("Act a%d\n", best_act+1);
}
