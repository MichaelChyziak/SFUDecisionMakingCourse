// Programmer: Michael Chyziak
// Date: Wednesday July 2, 2018
//
// Program description:
// 	TODO
// Required supplementary files:
// 	wine.data
// Got data set from here: 
//	http://archive.ics.uci.edu/ml/machine-learning-databases/wine/wine.data
// g++ least_squares.cc -lm -lglut -lGL -lGLU -o least_squares

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

// Global constants
const int num_wine_data_points = 178; 

// Function Declarations
int parseWineData(float wine_data[num_wine_data_points][3]);
void detectKeyboard(unsigned char key, int x, int y);
void displayGUI(int argc, char **argv);
void drawScreen();
void displayString(float x, float y, string input);
void computeCorrelationCoefficient(float wine_data[num_wine_data_points][3], float wine_scatterplot_correlation_coefficient[3][3], float wine_mean[3], float wine_variance[3]);
void calculateMean(float wine_data[num_wine_data_points][3], float wine_mean[3]);
void calculateVariance(float wine_data[num_wine_data_points][3], float wine_mean[3], float wine_variance[3]);
void calculateCovariance(float wine_data[num_wine_data_points][3], float wine_covariance[3][3], float wine_mean[3], float wine_variance[3]);
void calculateLeastSquares(float wine_scatterplot_correlation_coefficient[3][3], float wine_mean[3], float wine_variance[3], float wine_least_squares[3][3][2]);

// Global Variables
float wine_data[num_wine_data_points][3]; // Know that there is exactly 178 data points with 3 variables (alcohol, colour intensity, and hue)
float wine_mean[3]; // The mean for each wine variable (alcohol, colour intensity, and hue)
float wine_variance[3]; // The variance for each wine variable (alcohol, colour intensity, and hue)
float wine_scatterplot_correlation_coefficient[3][3]; // Correlation coefficient for each scatterplot in the 3x3 matix of scatterplots shown
float wine_covariance[3][3]; // covariance for each scatterplot in the 3x3 matrix
float wine_least_squares[3][3][2]; // Least squares regression line y = a + bx for each scatterplot, a = index 0, b = index 1

int main(int argc, char **argv) {

	// Variables
	int status;

	// Parses input from file and stores into user_data array
	// If cannot read from file it ends the program
	status = parseWineData(wine_data);
	if (status == -1) {
		return status;		
	}
	
	// Calculate the mean and variance of all 3 wine variables
	calculateMean(wine_data, wine_mean);
	calculateVariance(wine_data, wine_mean, wine_variance);

	// Calculates correlation coefficiants for each off diagonal scatterplot
	computeCorrelationCoefficient(wine_data, wine_scatterplot_correlation_coefficient, wine_mean, wine_variance);

	// Calculates the associated 3x3 covariance matrix
	calculateCovariance(wine_data, wine_covariance, wine_mean, wine_variance);

	// Calculates the least squares regression line for each scatterplot
	calculateLeastSquares(wine_scatterplot_correlation_coefficient, wine_mean, wine_variance, wine_least_squares);

	// Run GUI
	displayGUI(argc, argv);
}

// Calculates the least squares regression line for each scatterplot
// y = a + bx;
// b = r * sqrt(var(x)) / sqrt(var(y));
// a = mean(y) - b * mean(x);
void calculateLeastSquares(float wine_scatterplot_correlation_coefficient[3][3], float wine_mean[3], float wine_variance[3], float wine_least_squares[3][3][2]) {
	
	// Variables
	int matrix_row_index;
	int matrix_col_index;
	float temp_b;
	float temp_a;

	// Calculates least squares regression line for off diagonal scatterplots
	for (matrix_row_index = 0; matrix_row_index < 3; matrix_row_index++) {
		for (matrix_col_index = 0; matrix_col_index < 3; matrix_col_index++) {
			// Ignore main diagonal
			if (matrix_row_index == matrix_col_index) {
				// Assign 0 for safety, but ignoring essentially
				wine_least_squares[matrix_row_index][matrix_col_index][0] = 0;
				wine_least_squares[matrix_row_index][matrix_col_index][1] = 0;
			}
			else {
				// Calculate b = r * sqrt(var(x)) / sqrt(var(y))
				temp_b = wine_scatterplot_correlation_coefficient[matrix_row_index][matrix_col_index] * sqrt(wine_variance[matrix_row_index]) / sqrt(wine_variance[matrix_col_index]);
				// Calculate a = mean(y) - b * mean(x)
				temp_a = wine_mean[matrix_row_index] - (temp_b * wine_mean[matrix_col_index]);
				// Assign to equation y = a + bx
				wine_least_squares[matrix_row_index][matrix_col_index][0] = temp_a;
				wine_least_squares[matrix_row_index][matrix_col_index][1] = temp_b;
			}
		}
	}
}

// Calculates the mean value for each wine data variable
void calculateMean(float wine_data[num_wine_data_points][3], float wine_mean[3]) {
        
	// Variables
	int wine_index;
	int wine_variable_index;
	int number_of_values;
	float sum_value;

	// Calculates mean for all 3 wine variables 
	for (wine_variable_index = 0; wine_variable_index < 3; wine_variable_index++) {
		// Calculates the sum of all of the values and the number of values taken
		sum_value = 0;
		number_of_values = 0;
		for (wine_index = 0; wine_index < num_wine_data_points; wine_index++) {
			sum_value += wine_data[wine_index][wine_variable_index];
			number_of_values++;
		}
		// Calculates the mean and stores it
		wine_mean[wine_variable_index] =  sum_value / (float) number_of_values;
	}
}

// Calculates the variance for each wine data variable
void calculateVariance(float wine_data[num_wine_data_points][3], float wine_mean[3], float wine_variance[3]) {

	// Variables
	int wine_index;
	int wine_variable_index;
	int number_of_values;
	float sum_value;
	float temp_sum_value;

	// Calculates the number of values in the histogram
	number_of_values = num_wine_data_points; // Already known this

	// Calculates the variance for all 3 wine variables
	for (wine_variable_index = 0; wine_variable_index < 3; wine_variable_index++) {
		sum_value = 0;
		temp_sum_value = 0;
		// Calculates the summation of (value - mean)^2
		for (wine_index = 0; wine_index < num_wine_data_points; wine_index++) {
			temp_sum_value = pow(wine_data[wine_index][wine_variable_index] - wine_mean[wine_variable_index], 2);
			sum_value += temp_sum_value;
		}

		// Calculates the variance
		wine_variance[wine_variable_index] = (1.0f / (number_of_values - 1)) * sum_value;
	}
}

// Calculates the 3x3 covariance matrix of the wine variables
void calculateCovariance(float wine_data[num_wine_data_points][3], float wine_covariance[3][3], float wine_mean[3], float wine_variance[3]) {

	// Variables
	int wine_row_index;
	int wine_col_index;
	int wine_index;
	float sum_value;
	float temp_sum_value;
	int number_of_values = num_wine_data_points;

	// Compute the correlation coefficiuent of the 3x3 covariance matrix
	for (wine_row_index = 0; wine_row_index < 3; wine_row_index++) {
		for (wine_col_index = 0; wine_col_index < 3; wine_col_index++) {
			sum_value = 0;
			temp_sum_value;
			// covariance of X,X is just the variance
			if (wine_row_index == wine_col_index) {
				wine_covariance[wine_row_index][wine_col_index] = wine_variance[wine_row_index];
			}
			else {
				for (wine_index = 0; wine_index < num_wine_data_points; wine_index++) {
					temp_sum_value = (wine_data[wine_index][wine_col_index] - wine_mean[wine_col_index]) * (wine_data[wine_index][wine_row_index] - wine_mean[wine_row_index]);
					sum_value += temp_sum_value;
				}

				// Calculates the covariance
				wine_covariance[wine_row_index][wine_col_index] = (1.0f / (number_of_values - 1)) * sum_value;
			}
		}
	}
}

// Computes the correlation coefficient for each off diagonal scatterplot
void computeCorrelationCoefficient(float wine_data[num_wine_data_points][3], float wine_scatterplot_correlation_coefficient[3][3], float wine_mean[3], float wine_variance[3]) {

	// Variables
	int wine_row_index;
	int wine_col_index;
	int wine_index;
	float temp_sum_value;
	int number_of_values = num_wine_data_points; // Already known this

	// Compute the correlation coefficient for each off diagonal scatterplot
	for (wine_row_index = 0; wine_row_index < 3; wine_row_index++) {
		for (wine_col_index = 0; wine_col_index < 3; wine_col_index++) {
			temp_sum_value = 0;
			for (wine_index = 0; wine_index < num_wine_data_points; wine_index++) {
				temp_sum_value += ((wine_data[wine_index][wine_col_index] - wine_mean[wine_col_index]) / sqrt(wine_variance[wine_col_index]) *
						((wine_data[wine_index][wine_row_index] - wine_mean[wine_row_index]) / sqrt(wine_variance[wine_row_index])));
			}
			wine_scatterplot_correlation_coefficient[wine_row_index][wine_col_index] = (1.0f / (number_of_values - 1)) * temp_sum_value;
		}
	}
}

// Parses file to get the appropriate wine data on the 3 relevant variables in an array
// Return -1 = error
// Return 0 = success
int parseWineData(float wine_data[num_wine_data_points][3]) {
	
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
        glutCreateWindow("ENSC482 Michael Chyziak - Homework 3");

        // Update depth buffer if test passes
        // glEnable(GL_DEPTH_TEST);

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
	float matrix_row_start = 0.85;
	float matrix_col_start = -0.85;
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
	int axis_index;
	float least_squares_y_start;
	float least_squares_y_end;
	float least_squares_x_start;
	float least_squares_x_end;

        // Set Background Color to a light grey
        glClearColor(0.4, 0.4, 0.4, 1.0);

        // Clear screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Reset transformations
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

	// Determine maximum and minimum variable values (alcohol, colour intensity, and hue)
	for (wine_index = 0; wine_index < num_wine_data_points; wine_index++) {
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
			glVertex2f(matrix_col_start + (matrix_col_index * (matrix_col_delta + matrix_col_buffer)), 
					matrix_row_start - (matrix_row_index * (matrix_row_delta + matrix_row_buffer)));
			glVertex2f((matrix_col_start + matrix_col_delta) + (matrix_col_index * (matrix_col_delta + matrix_col_buffer)), 
					matrix_row_start - (matrix_row_index * (matrix_row_delta + matrix_row_buffer)));
			glEnd();
			glBegin(GL_LINES);
			glColor3f(0, 0, 0);
			glVertex2f((matrix_col_start + matrix_col_delta) + (matrix_col_index * (matrix_col_delta + matrix_col_buffer)), 
					matrix_row_start - (matrix_row_index * (matrix_row_delta + matrix_row_buffer)));
			glVertex2f((matrix_col_start + matrix_col_delta) + (matrix_col_index * (matrix_col_delta + matrix_col_buffer)), 
					(matrix_row_start - matrix_row_delta) - (matrix_row_index * (matrix_row_delta + matrix_row_buffer)));
			glEnd();
			glBegin(GL_LINES);
			glColor3f(0, 0, 0);
			glVertex2f((matrix_col_start + matrix_col_delta) + (matrix_col_index * (matrix_col_delta + matrix_col_buffer)), 
					(matrix_row_start - matrix_row_delta) - (matrix_row_index * (matrix_row_delta + matrix_row_buffer)));
			glVertex2f(matrix_col_start + (matrix_col_index * (matrix_col_delta + matrix_col_buffer)), 
					(matrix_row_start - matrix_row_delta) - (matrix_row_index * (matrix_row_delta + matrix_row_buffer)));
			glEnd();
			glBegin(GL_LINES);
			glColor3f(0, 0, 0);
			glVertex2f(matrix_col_start + (matrix_col_index * (matrix_col_delta + matrix_col_buffer)), 
					(matrix_row_start - matrix_row_delta) - (matrix_row_index * (matrix_row_delta + matrix_row_buffer)));
			glVertex2f(matrix_col_start + (matrix_col_index * (matrix_col_delta + matrix_col_buffer)), 
					matrix_row_start - (matrix_row_index * (matrix_row_delta + matrix_row_buffer)));
			glEnd();
			// Axis bars, values, pearson correlation coefficient (r), and covariance (cov)
			// Don't add to main diagonal
			if (matrix_row_index == matrix_col_index) {
				// Display only Pearson correlation coefficient (r) and covariance (cov)
				display_string = "r=" + to_string(wine_scatterplot_correlation_coefficient[matrix_row_index][matrix_col_index]) + ",cov=" + 
							to_string(wine_covariance[matrix_row_index][matrix_col_index]);
				displayString(matrix_col_start + (matrix_col_index * (matrix_col_delta + matrix_col_buffer)) + 0.08f,
						matrix_row_start - (matrix_row_index * (matrix_row_delta + matrix_row_buffer)) + 0.005f, 
						display_string);
			}
			else {
				for (axis_index = 0; axis_index < 6; axis_index++) {
					// Y-axis
					glBegin(GL_LINES);
					glColor3f(0, 0, 0);
					glVertex2f(matrix_col_start + (matrix_col_index * (matrix_col_delta + matrix_col_buffer)),
							(matrix_row_start - matrix_row_delta) - (matrix_row_index * (matrix_row_delta + matrix_row_buffer)) + (axis_index * matrix_row_delta / 5));
					glVertex2f(matrix_col_start + (matrix_col_index * (matrix_col_delta + matrix_col_buffer)) - 0.02f,
							(matrix_row_start - matrix_row_delta) - (matrix_row_index * (matrix_row_delta + matrix_row_buffer)) + (axis_index * matrix_row_delta / 5));
					glEnd();
					display_string = to_string(axis_index / 5.0f * max_wine_variable_value[matrix_row_index]);
					display_string.resize(4);
					displayString(matrix_col_start + (matrix_col_index * (matrix_col_delta + matrix_col_buffer)) - 0.08f,
							(matrix_row_start - matrix_row_delta)- (matrix_row_index * (matrix_row_delta + matrix_row_buffer)) + (axis_index * matrix_row_delta / 5) - 0.01f, 
							display_string);
					// X-axis
					glBegin(GL_LINES);
					glColor3f(0, 0, 0);
					glVertex2f(matrix_col_start + (matrix_col_index * (matrix_col_delta + matrix_col_buffer)) + (axis_index * matrix_col_delta / 5),
							(matrix_row_start - matrix_row_delta) - (matrix_row_index * (matrix_row_delta + matrix_row_buffer)));
					glVertex2f(matrix_col_start + (matrix_col_index * (matrix_col_delta + matrix_col_buffer)) + (axis_index * matrix_col_delta / 5),
							(matrix_row_start - matrix_row_delta)- (matrix_row_index * (matrix_row_delta + matrix_row_buffer)) - 0.02f);
					glEnd();
					display_string = to_string(axis_index / 5.0f * max_wine_variable_value[matrix_col_index]);
					display_string.resize(4);
					displayString(matrix_col_start + (matrix_col_index * (matrix_col_delta + matrix_col_buffer)) + (axis_index * matrix_col_delta / 5) - 0.03f,
							(matrix_row_start - matrix_row_delta)- (matrix_row_index * (matrix_row_delta + matrix_row_buffer)) - 0.05f, display_string);
				}
				// Pearson correlation coefficient (r) and covariance (cov)
				display_string = "r=" + to_string(wine_scatterplot_correlation_coefficient[matrix_row_index][matrix_col_index]) + ",cov=" + 
							to_string(wine_covariance[matrix_row_index][matrix_col_index]);
				displayString(matrix_col_start + (matrix_col_index * (matrix_col_delta + matrix_col_buffer)) + 0.08f,
						matrix_row_start - (matrix_row_index * (matrix_row_delta + matrix_row_buffer)) + 0.005f, 
						display_string);
			}
		}
	}

	// Draw each scatter plot diagram (bar graph for main diagonal)
	// Each data point is represented by an 'x')
	for (matrix_row_index = 0; matrix_row_index < 3; matrix_row_index++) {
		for (matrix_col_index = 0; matrix_col_index < 3; matrix_col_index++) {
			if (matrix_row_index != matrix_col_index) {
				for (wine_index = 0; wine_index < num_wine_data_points; wine_index++) {
					// Find the points relative value in the scatter plot (between 0 and x and y delta's)
					relative_scatterplot_x_value = wine_data[wine_index][matrix_col_index] / max_wine_variable_value[matrix_col_index] * matrix_col_delta;
					relative_scatterplot_y_value = wine_data[wine_index][matrix_row_index] / max_wine_variable_value[matrix_row_index] * matrix_row_delta;
					// Draw 'x'
					glBegin(GL_LINES);
					glColor3f(0, 0, 1);
					glVertex2f(matrix_col_start + (matrix_col_index * (matrix_col_delta + matrix_col_buffer)) + relative_scatterplot_x_value - 0.01f, 
							(matrix_row_start - matrix_row_delta) - (matrix_row_index * (matrix_row_delta + matrix_row_buffer)) + relative_scatterplot_y_value + 0.01f) ;
					glVertex2f(matrix_col_start + (matrix_col_index * (matrix_col_delta + matrix_col_buffer)) + relative_scatterplot_x_value + 0.01f, 
							(matrix_row_start - matrix_row_delta) - (matrix_row_index * (matrix_row_delta + matrix_row_buffer)) + relative_scatterplot_y_value - 0.01f) ;
					glEnd();
					glBegin(GL_LINES);
					glColor3f(0, 0, 1);
					glVertex2f(matrix_col_start + (matrix_col_index * (matrix_col_delta + matrix_col_buffer)) + relative_scatterplot_x_value - 0.01f, 
							(matrix_row_start - matrix_row_delta) - (matrix_row_index * (matrix_row_delta + matrix_row_buffer)) + relative_scatterplot_y_value - 0.01f) ;
					glVertex2f(matrix_col_start + (matrix_col_index * (matrix_col_delta + matrix_col_buffer)) + relative_scatterplot_x_value + 0.01f, 
							(matrix_row_start - matrix_row_delta) - (matrix_row_index * (matrix_row_delta + matrix_row_buffer)) + relative_scatterplot_y_value + 0.01f) ;
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
				displayString(matrix_col_start + (matrix_col_index * (matrix_col_delta + matrix_col_buffer)) + 0.1f,
						matrix_row_start - (matrix_row_index * (matrix_row_delta + matrix_row_buffer)) - (matrix_row_delta / 2), display_string);
			}
		}
	}

	// Draw least squares regression line for each off diagonal scatterplot
	for (matrix_row_index = 0; matrix_row_index < 3; matrix_row_index++) {
		for (matrix_col_index = 0; matrix_col_index < 3; matrix_col_index++) {
			// Ignore main diagonal
			if (matrix_row_index == matrix_col_index) {
				// Do nothing
			}
			else {
				// Find y when x = 0 and x = max, then draw line from start to end y's
				// If intercept below y = 0, start line at when y = 0
				if (wine_least_squares[matrix_row_index][matrix_col_index][0] < 0.0f) {
					least_squares_y_start = 0;
					least_squares_x_start = -1.0f * wine_least_squares[matrix_row_index][matrix_col_index][0] / wine_least_squares[matrix_row_index][matrix_col_index][1];
				}
				else {
					least_squares_y_start = wine_least_squares[matrix_row_index][matrix_col_index][0]; // x = 0, y-intercept
					least_squares_x_start = 0;
				}
				least_squares_x_end = max_wine_variable_value[matrix_col_index];
				least_squares_y_end = wine_least_squares[matrix_row_index][matrix_col_index][0] + 
							(wine_least_squares[matrix_row_index][matrix_col_index][1] * max_wine_variable_value[matrix_col_index]); // x = max
				// Change y values to be relative values compared to scatterplot size
				least_squares_x_start = least_squares_x_start / max_wine_variable_value[matrix_col_index] * matrix_col_delta; // relative value
				least_squares_x_end = least_squares_x_end / max_wine_variable_value[matrix_col_index] * matrix_col_delta; // relative value
				least_squares_y_start = least_squares_y_start / max_wine_variable_value[matrix_row_index] * matrix_row_delta; // relative value
				least_squares_y_end = least_squares_y_end / max_wine_variable_value[matrix_row_index] * matrix_row_delta; // relative value
				glBegin(GL_LINES);
				glColor3f(1, 0, 0);
				glVertex2f(matrix_col_start + (matrix_col_index * (matrix_col_delta + matrix_col_buffer)) + least_squares_x_start, 
						(matrix_row_start - matrix_row_delta) - (matrix_row_index * (matrix_row_delta + matrix_row_buffer)) + least_squares_y_start);
				glVertex2f(matrix_col_start + (matrix_col_index * (matrix_col_delta + matrix_col_buffer)) + least_squares_x_end,
						(matrix_row_start - matrix_row_delta) - (matrix_row_index * (matrix_row_delta + matrix_row_buffer)) + least_squares_y_end);
				glEnd();
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
