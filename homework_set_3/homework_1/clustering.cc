// Programmer: Michael Chyziak
// Date: Wednesday July 27, 2018
//
// Program description:
// 	Uses k-means clustering algorithm on the wine data of alcohol vs hue to cluster it into 3 different clusters.
// Required supplementary files:
// 	wine.data
// Got data set from here: 
//	http://archive.ics.uci.edu/ml/machine-learning-databases/wine/wine.data
// g++ clustering.cc -lm -lglut -lGL -lGLU -o clustering

// Standard libraries
#include <iostream>
#include <string>
#include <stdlib.h>
#include <cstring>
#include <math.h>
#include <vector>

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
int parseWineData();
void detectKeyboard(unsigned char key, int x, int y);
void displayGUI(int argc, char **argv);
void drawScreen();
void displayString(float x, float y, string input);
float euclideanDistanceSquared(pair<float, float> pair_1, pair<float, float> pair_2);

// Global Variables
vector<pair<float, float>> wine_data_original; // Know that there is exactly 178 data points with 2 variables (alcohol, and hue)
vector<pair<float, float>> wine_data_cluster_1; // Each vector holds the data points belonging to each cluster
vector<pair<float, float>> wine_data_cluster_2; // the pair is to hold the coordinates of x (first) and y (second)
vector<pair<float, float>> wine_data_cluster_3;
pair<float, float> wine_data_mean_1;
pair<float, float> wine_data_mean_2;
pair<float, float> wine_data_mean_3;
int iteration = 0;
bool done = false;

int main(int argc, char **argv) {

	// Variables
	int status;

	// Parses input from file and stores into user_data array
	// If cannot read from file it ends the program
	status = parseWineData();
	if (status == -1) {
		return status;		
	}

	// Run GUI
	displayGUI(argc, argv);
}

// Parses file to get the appropriate wine data on the 2 relevant variables in an array
// Return -1 = error
// Return 0 = success
int parseWineData() {
	
	// Variables
	FILE* file_pointer;
	char line_buffer[255];
	unsigned int nth_number = 1; // The 'n-th' number read in the line
	const char delimiter[2] = ",";
	int number;
	char* token_value;
	int wine_data_index = 0;
	float wine_data_first;
	float wine_data_second;

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
					wine_data_first = atof(token_value);
				} 
	       		else if (nth_number == 12) {
					wine_data_second = atof(token_value);
				} 
				else { // nth_number < 10 and !=2, or error
					// Ignore value
				}
				nth_number++;
				// Continue until hit next delimiter
				token_value = strtok(NULL, delimiter);
			}
			// Add data point
			wine_data_original.push_back(make_pair(wine_data_first, wine_data_second));
			if (wine_data_original.size() == num_wine_data_points) {
				break;
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

	// Select the 3 initial means to be first 3 data points
	wine_data_mean_1 = make_pair(wine_data_original[0].first, wine_data_original[0].second);
	wine_data_mean_2 = make_pair(wine_data_original[1].first, wine_data_original[1].second);
	wine_data_mean_3 = make_pair(wine_data_original[2].first, wine_data_original[2].second);

	// Close the file
	fclose(file_pointer);
}

// Pressing escape closes the GUI
void detectKeyboard(unsigned char key, int x, int y) {
	int wine_data_index;
	float euclidean_cluster_1;
	float euclidean_cluster_2;
	float euclidean_cluster_3;
	float mean_x_value;
	float mean_y_value;
	pair<float, float> wine_data_mean_1_old;
	pair<float, float> wine_data_mean_2_old;
	pair<float, float> wine_data_mean_3_old;

    // If escape is pressed, quit the GUI
    if (key == 27) {
        printf("Closing GUI.\n");
        glutLeaveMainLoop();
    }
    
    // If Enter key is pressed, do the next k-means iteration
    if (key == 13 && done == false) {
    	iteration++;

    	// Clear current wine data cluster
    	wine_data_cluster_1.clear(); // Each vector holds the data points belonging to each cluster
		wine_data_cluster_2.clear(); // the pair is to hold the coordinates of x (first) and y (second)
		wine_data_cluster_3.clear();

    	// Assign data points to the nearest cluster mean
    	for (wine_data_index = 0; wine_data_index < num_wine_data_points; wine_data_index++) {
    		euclidean_cluster_1 = euclideanDistanceSquared(wine_data_original[wine_data_index], wine_data_mean_1);
    		euclidean_cluster_2 = euclideanDistanceSquared(wine_data_original[wine_data_index], wine_data_mean_2);
    		euclidean_cluster_3 = euclideanDistanceSquared(wine_data_original[wine_data_index], wine_data_mean_3);
    		if (euclidean_cluster_1 < euclidean_cluster_2) {
    			if (euclidean_cluster_1 < euclidean_cluster_3) {
    				wine_data_cluster_1.push_back(wine_data_original[wine_data_index]);
    			}
    			else {
    				wine_data_cluster_3.push_back(wine_data_original[wine_data_index]);
    			}
    		}
    		else {
    			if (euclidean_cluster_2 < euclidean_cluster_3) {
					wine_data_cluster_2.push_back(wine_data_original[wine_data_index]);
    			}
    			else {
    				wine_data_cluster_3.push_back(wine_data_original[wine_data_index]);
    			}
    		}
    	}

    	// Store previous mean values
    	wine_data_mean_1_old.first = wine_data_mean_1.first;
    	wine_data_mean_1_old.second = wine_data_mean_1.second;
    	wine_data_mean_2_old.first = wine_data_mean_2.first;
    	wine_data_mean_2_old.second = wine_data_mean_2.second;
    	wine_data_mean_3_old.first = wine_data_mean_3.first;
    	wine_data_mean_3_old.second = wine_data_mean_3.second;

    	// Recalculate means
    	for (wine_data_index = 0, mean_x_value = 0, mean_y_value = 0; wine_data_index < wine_data_cluster_1.size(); wine_data_index++) {
    		mean_x_value += wine_data_cluster_1[wine_data_index].first;
    		mean_y_value += wine_data_cluster_1[wine_data_index].second;
    	}
    	wine_data_mean_1.first = mean_x_value / wine_data_cluster_1.size();
    	wine_data_mean_1.second = mean_y_value / wine_data_cluster_1.size();
    	for (wine_data_index = 0, mean_x_value = 0, mean_y_value = 0; wine_data_index < wine_data_cluster_2.size(); wine_data_index++) {
    		mean_x_value += wine_data_cluster_2[wine_data_index].first;
    		mean_y_value += wine_data_cluster_2[wine_data_index].second;
    	}
    	wine_data_mean_2.first = mean_x_value / wine_data_cluster_2.size();
    	wine_data_mean_2.second = mean_y_value / wine_data_cluster_2.size();
    	for (wine_data_index = 0, mean_x_value = 0, mean_y_value = 0; wine_data_index < wine_data_cluster_3.size(); wine_data_index++) {
    		mean_x_value += wine_data_cluster_3[wine_data_index].first;
    		mean_y_value += wine_data_cluster_3[wine_data_index].second;
    	}
    	wine_data_mean_3.first = mean_x_value / wine_data_cluster_3.size();
    	wine_data_mean_3.second = mean_y_value / wine_data_cluster_3.size();

    	// If mean doesn't change, end the algorithm
    	if (wine_data_mean_1_old.first == wine_data_mean_1.first && wine_data_mean_1_old.second == wine_data_mean_1.second && wine_data_mean_2_old.first == wine_data_mean_2.first &&
    		wine_data_mean_2_old.second == wine_data_mean_2.second && wine_data_mean_3_old.first == wine_data_mean_3.first && wine_data_mean_3_old.second == wine_data_mean_3.second) {
    		done = true;
    	}

    	// Redisplay
		glutPostRedisplay();
    }

    // If Space bar is pressed, reset program to start values
    if (key == 32) {
    	done = false;
    	iteration = 0;
    	wine_data_cluster_1.clear(); // Each vector holds the data points belonging to each cluster
		wine_data_cluster_2.clear(); // the pair is to hold the coordinates of x (first) and y (second)
		wine_data_cluster_3.clear();
		wine_data_mean_1 = make_pair(wine_data_original[0].first, wine_data_original[0].second);
		wine_data_mean_2 = make_pair(wine_data_original[1].first, wine_data_original[1].second);
		wine_data_mean_3 = make_pair(wine_data_original[2].first, wine_data_original[2].second);
		glutPostRedisplay();
    }
}

// Calculates the Euclidean Distance between to points (2d)
// Points represented as pairs where x = first and y = second
// Squares it so that it cancels the square root
float euclideanDistanceSquared(pair<float, float> pair_1, pair<float, float> pair_2) {
	float x_value;
	float y_value;

	x_value = pow(pair_1.first - pair_2.first, 2.0f);
	y_value = pow(pair_1.second - pair_2.second, 2.0f);

	return (x_value + y_value);
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
    glutCreateWindow("ENSC482 Michael Chyziak - Homework 1");

    // Update depth buffer if test passes
    // glEnable(GL_DEPTH_TEST);

    // Callback functions
    glutDisplayFunc(drawScreen);
    glutKeyboardFunc(detectKeyboard);

    // Pass control to GLUT for events
    glutMainLoop();
}

// Draws the scatter plot of the 2 wine variables (alcohol and hue)
void drawScreen() {

	// Variables
	float matrix_y_start = 0.65;
	float matrix_x_start = -0.85;
	float matrix_y_delta = 1.5;
	float matrix_x_delta = 1.5;
	float max_wine_variable_value[2] = {0, 0}; // for 2 variables: alcohol and hue
	float min_wine_variable_value[2] = {100, 100}; // for 2 variables: alcohol and hue
	int wine_index;
	int wine_variable_index;
	int axis_index;
	int y_axis_index;
	char char_string;
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

	// Determine maximum and minimum variable values (alcohol and hue)
	// Alcohol min/max
	for (wine_index = 0; wine_index < num_wine_data_points; wine_index++) {
		if (wine_data_original[wine_index].first > max_wine_variable_value[0]) {
			max_wine_variable_value[0] = wine_data_original[wine_index].first;
		}
		if (wine_data_original[wine_index].first < min_wine_variable_value[0]) {
			min_wine_variable_value[0] = wine_data_original[wine_index].first;
		}
	}
	// Hue min/max
	for (wine_index = 0; wine_index < num_wine_data_points; wine_index++) {
		if (wine_data_original[wine_index].second > max_wine_variable_value[1]) {
			max_wine_variable_value[1] = wine_data_original[wine_index].second;
		}
		if (wine_data_original[wine_index].second < min_wine_variable_value[1]) {
			min_wine_variable_value[1] = wine_data_original[wine_index].second;
		}
	}

	// Scatter plot diagram
	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	glVertex2f(matrix_x_start, matrix_y_start);
	glVertex2f(matrix_x_start + matrix_x_delta, matrix_y_start);
	glEnd();
	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	glVertex2f(matrix_x_start + matrix_x_delta, matrix_y_start);
	glVertex2f(matrix_x_start + matrix_x_delta, matrix_y_start - matrix_y_delta);
	glEnd();
	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	glVertex2f(matrix_x_start + matrix_x_delta, matrix_y_start - matrix_y_delta);
	glVertex2f(matrix_x_start, matrix_y_start - matrix_y_delta);
	glEnd();
	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	glVertex2f(matrix_x_start, matrix_y_start - matrix_y_delta);
	glVertex2f(matrix_x_start, matrix_y_start);
	glEnd();

	// Axis bars and values
	for (axis_index = 0; axis_index < 6; axis_index++) {
		// Y-axis (hue)
		glBegin(GL_LINES);
		glColor3f(0, 0, 0);
		glVertex2f(matrix_x_start, matrix_y_start - matrix_y_delta + (axis_index * matrix_y_delta / 5));
		glVertex2f(matrix_x_start - 0.02f, matrix_y_start - matrix_y_delta + (axis_index * matrix_y_delta / 5));
		glEnd();
		display_string = to_string((axis_index * (max_wine_variable_value[1] - min_wine_variable_value[1]) / 5.0f) + min_wine_variable_value[1]);
		display_string.resize(4);
		displayString(matrix_x_start - 0.08f, matrix_y_start - matrix_y_delta + (axis_index * matrix_y_delta / 5) - 0.01f, display_string);
		// X-axis (alcohol)
		glBegin(GL_LINES);
		glColor3f(0, 0, 0);
		glVertex2f(matrix_x_start + (axis_index * matrix_x_delta / 5), matrix_y_start - matrix_y_delta);
		glVertex2f(matrix_x_start + (axis_index * matrix_x_delta / 5), matrix_y_start - matrix_y_delta - 0.02f);
		glEnd();
		display_string = to_string((axis_index * (max_wine_variable_value[0] - min_wine_variable_value[0]) / 5.0f) + min_wine_variable_value[0]);
		display_string.resize(4);
		displayString(matrix_x_start + (axis_index * matrix_x_delta / 5) - 0.03f, matrix_y_start - matrix_y_delta - 0.05f, display_string);
	}
	
	// Display X-axis and Y-axis names (alcohol, and hue)
	displayString(-0.2f, matrix_y_start - matrix_y_delta - 0.1f, "Wine Alcohol");
	displayString(matrix_x_start - 0.12f, 0.2f, "W");
	displayString(matrix_x_start - 0.12f, 0.15f, "i");
	displayString(matrix_x_start - 0.12f, 0.1f, "n");
	displayString(matrix_x_start - 0.12f, 0.05f, "e");
	displayString(matrix_x_start - 0.12f, -0.05f, "H");
	displayString(matrix_x_start - 0.12f, -0.1f, "u");
	displayString(matrix_x_start - 0.12f, -0.15f, "e");

	// Draw scatter plot diagram
	// Each data point is represented by an 'x'
	if (iteration == 0) {
		for (wine_index = 0; wine_index < num_wine_data_points; wine_index++) {
			// Find the points relative value in the scatter plot (between 0 and x and y delta's)
			relative_scatterplot_x_value = (wine_data_original[wine_index].first - min_wine_variable_value[0]) / (max_wine_variable_value[0] - min_wine_variable_value[0]) * matrix_x_delta;
			relative_scatterplot_y_value = (wine_data_original[wine_index].second - min_wine_variable_value[1]) / (max_wine_variable_value[1] - min_wine_variable_value[1]) * matrix_y_delta;
			// Draw 'x'
			glBegin(GL_LINES);
			glColor3f(0, 0, 0);
			glVertex2f(matrix_x_start + relative_scatterplot_x_value - 0.01f, matrix_y_start - matrix_y_delta + relative_scatterplot_y_value + 0.01f);
			glVertex2f(matrix_x_start + relative_scatterplot_x_value + 0.01f, matrix_y_start - matrix_y_delta + relative_scatterplot_y_value - 0.01f);
			glEnd();
			glBegin(GL_LINES);
			glColor3f(0, 0, 0);
			glVertex2f(matrix_x_start + relative_scatterplot_x_value - 0.01f, matrix_y_start - matrix_y_delta + relative_scatterplot_y_value - 0.01f);
			glVertex2f(matrix_x_start + relative_scatterplot_x_value + 0.01f, matrix_y_start - matrix_y_delta + relative_scatterplot_y_value + 0.01f);
			glEnd();
		}
	}
	else {
		for (wine_index = 0; wine_index < wine_data_cluster_1.size(); wine_index++) {
			// Find the points relative value in the scatter plot (between 0 and x and y delta's)
			relative_scatterplot_x_value = (wine_data_cluster_1[wine_index].first - min_wine_variable_value[0]) / (max_wine_variable_value[0] - min_wine_variable_value[0]) * matrix_x_delta;
			relative_scatterplot_y_value = (wine_data_cluster_1[wine_index].second - min_wine_variable_value[1]) / (max_wine_variable_value[1] - min_wine_variable_value[1]) * matrix_y_delta;
			// Draw 'x'
			glBegin(GL_LINES);
			glColor3f(1, 0, 0);
			glVertex2f(matrix_x_start + relative_scatterplot_x_value - 0.01f, matrix_y_start - matrix_y_delta + relative_scatterplot_y_value + 0.01f);
			glVertex2f(matrix_x_start + relative_scatterplot_x_value + 0.01f, matrix_y_start - matrix_y_delta + relative_scatterplot_y_value - 0.01f);
			glEnd();
			glBegin(GL_LINES);
			glColor3f(1, 0, 0);
			glVertex2f(matrix_x_start + relative_scatterplot_x_value - 0.01f, matrix_y_start - matrix_y_delta + relative_scatterplot_y_value - 0.01f);
			glVertex2f(matrix_x_start + relative_scatterplot_x_value + 0.01f, matrix_y_start - matrix_y_delta + relative_scatterplot_y_value + 0.01f);
			glEnd();
		}
		for (wine_index = 0; wine_index < wine_data_cluster_2.size(); wine_index++) {
			// Find the points relative value in the scatter plot (between 0 and x and y delta's)
			relative_scatterplot_x_value = (wine_data_cluster_2[wine_index].first - min_wine_variable_value[0]) / (max_wine_variable_value[0] - min_wine_variable_value[0]) * matrix_x_delta;
			relative_scatterplot_y_value = (wine_data_cluster_2[wine_index].second - min_wine_variable_value[1]) / (max_wine_variable_value[1] - min_wine_variable_value[1]) * matrix_y_delta;
			// Draw 'x'
			glBegin(GL_LINES);
			glColor3f(0, 1, 0);
			glVertex2f(matrix_x_start + relative_scatterplot_x_value - 0.01f, matrix_y_start - matrix_y_delta + relative_scatterplot_y_value + 0.01f);
			glVertex2f(matrix_x_start + relative_scatterplot_x_value + 0.01f, matrix_y_start - matrix_y_delta + relative_scatterplot_y_value - 0.01f);
			glEnd();
			glBegin(GL_LINES);
			glColor3f(0, 1, 0);
			glVertex2f(matrix_x_start + relative_scatterplot_x_value - 0.01f, matrix_y_start - matrix_y_delta + relative_scatterplot_y_value - 0.01f);
			glVertex2f(matrix_x_start + relative_scatterplot_x_value + 0.01f, matrix_y_start - matrix_y_delta + relative_scatterplot_y_value + 0.01f);
			glEnd();
		}
		for (wine_index = 0; wine_index < wine_data_cluster_3.size(); wine_index++) {
			// Find the points relative value in the scatter plot (between 0 and x and y delta's)
			relative_scatterplot_x_value = (wine_data_cluster_3[wine_index].first - min_wine_variable_value[0]) / (max_wine_variable_value[0] - min_wine_variable_value[0]) * matrix_x_delta;
			relative_scatterplot_y_value = (wine_data_cluster_3[wine_index].second - min_wine_variable_value[1]) / (max_wine_variable_value[1] - min_wine_variable_value[1]) * matrix_y_delta;
			// Draw 'x'
			glBegin(GL_LINES);
			glColor3f(0, 0, 1);
			glVertex2f(matrix_x_start + relative_scatterplot_x_value - 0.01f, matrix_y_start - matrix_y_delta + relative_scatterplot_y_value + 0.01f);
			glVertex2f(matrix_x_start + relative_scatterplot_x_value + 0.01f, matrix_y_start - matrix_y_delta + relative_scatterplot_y_value - 0.01f);
			glEnd();
			glBegin(GL_LINES);
			glColor3f(0, 0, 1);
			glVertex2f(matrix_x_start + relative_scatterplot_x_value - 0.01f, matrix_y_start - matrix_y_delta + relative_scatterplot_y_value - 0.01f);
			glVertex2f(matrix_x_start + relative_scatterplot_x_value + 0.01f, matrix_y_start - matrix_y_delta + relative_scatterplot_y_value + 0.01f);
			glEnd();
		}
	}

	// Draw mean of data points
	relative_scatterplot_x_value = (wine_data_mean_1.first - min_wine_variable_value[0]) / (max_wine_variable_value[0] - min_wine_variable_value[0]) * matrix_x_delta;
	relative_scatterplot_y_value = (wine_data_mean_1.second - min_wine_variable_value[1]) / (max_wine_variable_value[1] - min_wine_variable_value[1]) * matrix_y_delta;
	glBegin(GL_POLYGON);
	glColor3f(1, 0, 0);
	glVertex2f(matrix_x_start + relative_scatterplot_x_value - 0.01f, matrix_y_start - matrix_y_delta + relative_scatterplot_y_value + 0.01f);
	glVertex2f(matrix_x_start + relative_scatterplot_x_value + 0.01f, matrix_y_start - matrix_y_delta + relative_scatterplot_y_value + 0.01f);
	glVertex2f(matrix_x_start + relative_scatterplot_x_value + 0.01f, matrix_y_start - matrix_y_delta + relative_scatterplot_y_value - 0.01f);
	glVertex2f(matrix_x_start + relative_scatterplot_x_value - 0.01f, matrix_y_start - matrix_y_delta + relative_scatterplot_y_value - 0.01f);
	glEnd();
	relative_scatterplot_x_value = (wine_data_mean_2.first - min_wine_variable_value[0]) / (max_wine_variable_value[0] - min_wine_variable_value[0]) * matrix_x_delta;
	relative_scatterplot_y_value = (wine_data_mean_2.second - min_wine_variable_value[1]) / (max_wine_variable_value[1] - min_wine_variable_value[1]) * matrix_y_delta;
	glBegin(GL_POLYGON);
	glColor3f(0, 1, 0);
	glVertex2f(matrix_x_start + relative_scatterplot_x_value - 0.01f, matrix_y_start - matrix_y_delta + relative_scatterplot_y_value + 0.01f);
	glVertex2f(matrix_x_start + relative_scatterplot_x_value + 0.01f, matrix_y_start - matrix_y_delta + relative_scatterplot_y_value + 0.01f);
	glVertex2f(matrix_x_start + relative_scatterplot_x_value + 0.01f, matrix_y_start - matrix_y_delta + relative_scatterplot_y_value - 0.01f);
	glVertex2f(matrix_x_start + relative_scatterplot_x_value - 0.01f, matrix_y_start - matrix_y_delta + relative_scatterplot_y_value - 0.01f);
	glEnd();
	relative_scatterplot_x_value = (wine_data_mean_3.first - min_wine_variable_value[0]) / (max_wine_variable_value[0] - min_wine_variable_value[0]) * matrix_x_delta;
	relative_scatterplot_y_value = (wine_data_mean_3.second - min_wine_variable_value[1]) / (max_wine_variable_value[1] - min_wine_variable_value[1]) * matrix_y_delta;
	glBegin(GL_POLYGON);
	glColor3f(0, 0, 1);
	glVertex2f(matrix_x_start + relative_scatterplot_x_value - 0.01f, matrix_y_start - matrix_y_delta + relative_scatterplot_y_value + 0.01f);
	glVertex2f(matrix_x_start + relative_scatterplot_x_value + 0.01f, matrix_y_start - matrix_y_delta + relative_scatterplot_y_value + 0.01f);
	glVertex2f(matrix_x_start + relative_scatterplot_x_value + 0.01f, matrix_y_start - matrix_y_delta + relative_scatterplot_y_value - 0.01f);
	glVertex2f(matrix_x_start + relative_scatterplot_x_value - 0.01f, matrix_y_start - matrix_y_delta + relative_scatterplot_y_value - 0.01f);
	glEnd();

	// Title, Instructions,and Legend
	displayString(-0.5f, 0.85f, "k-means clustering of Wine Alcohol vs Wine Hue (Iteration: " + to_string(iteration) + ")");
	if (done == false) {
		displayString(-0.25f, 0.8f, "To increase iteration: Press Enter");
	}
	else {
		displayString(-0.25f, 0.8f, "Algorithm Complete at Iteration " + to_string(iteration));
	}
	displayString(-0.25f, 0.75f, "To reset program: Press Spacebar");
	displayString(-0.25f, 0.7f, "To quit program: Press Escape");
	displayString(0.7f, 0.6f, "Legend");
	displayString(0.75f, 0.55f, "Original Data");
	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	glVertex2f(0.72, 0.58);
	glVertex2f(0.74, 0.56);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(0.72, 0.56);
	glVertex2f(0.74, 0.58);
	glEnd();
	displayString(0.75f, 0.48f, "Cluster 1 Data");
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex2f(0.72, 0.51);
	glVertex2f(0.74, 0.49);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(0.72, 0.49);
	glVertex2f(0.74, 0.51);
	glEnd();
	displayString(0.75f, 0.41f, "Cluster 2 Data");
	glBegin(GL_LINES);
	glColor3f(0, 1, 0);
	glVertex2f(0.72, 0.44);
	glVertex2f(0.74, 0.42);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(0.72, 0.42);
	glVertex2f(0.74, 0.44);
	glEnd();
	displayString(0.75f, 0.34f, "Cluster 3 Data");
	glBegin(GL_LINES);
	glColor3f(0, 0, 1);
	glVertex2f(0.72, 0.37);
	glVertex2f(0.74, 0.35);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(0.72, 0.35);
	glVertex2f(0.74, 0.37);
	glEnd();
	displayString(0.75f, 0.27f, "Cluster 1 Mean");
	glBegin(GL_POLYGON);
	glColor3f(1, 0, 0);
	glVertex2f(0.72, 0.30);
	glVertex2f(0.74, 0.30);
	glVertex2f(0.74, 0.28);
	glVertex2f(0.72, 0.28);
	glEnd();
	displayString(0.75f, 0.2f, "Cluster 2 Mean");
	glBegin(GL_POLYGON);
	glColor3f(0, 1, 0);
	glVertex2f(0.72, 0.23);
	glVertex2f(0.74, 0.23);
	glVertex2f(0.74, 0.21);
	glVertex2f(0.72, 0.21);
	glEnd();
	displayString(0.75f, 0.13f, "Cluster 3 Mean");
	glBegin(GL_POLYGON);
	glColor3f(0, 0, 1);
	glVertex2f(0.72, 0.16);
	glVertex2f(0.74, 0.16);
	glVertex2f(0.74, 0.14);
	glVertex2f(0.72, 0.14);
	glEnd();

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
