// Programmer: Michael Chyziak
// Date: Wednesday July 5, 2018
//
// Program description:
// 	TODO
// Required supplementary files:
// 	TODO
// Got SFU burnaby campus map from here (un-cropped): 
// 	http://www.sfu.ca/content/dam/sfu/fs/files/Campus%20maps/Burnaby%20aerial%20map.pdf
// TODO THIS one included doing "sudo apt install fltk1.3-dev" to get fltk (version 1.3)
// // TODO THIS IS WRONG -> MAKE IT PROPER ->~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~g++ sfu_graph.cc -lm -lglut -lGL -lGLU -o sfu_graph

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

// FLTK Includes (version 1.3)
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Shared_Image.H>
#include <Fl/Fl_PNG_Image.H>
#include <Fl/Fl_Group.H>
#include <Fl/fl_draw.H>

using namespace std;

// Global constants
const int number_of_nodes = 10;
// Node index [i][j]: i = node value. node j = 1 if i and j are connected by an edge, 0 if not
const int node_connections[10][10] = 	{{0, 0, 1, 1, 1, 0, 1, 1, 0, 0}, // Gym
					{0, 0, 0, 0, 0, 0, 1, 1, 0, 0}, // 429 Class
					{1, 0, 0, 0, 0, 0, 1, 1, 0, 0}, // 482 Class
					{1, 0, 0, 0, 0, 0, 1, 1, 0, 0}, // Lockers
					{1, 0, 0, 0, 0, 0, 1, 0, 0, 0}, // Mackenzie Cafe
					{0, 0, 0, 0, 0, 0, 1, 1, 0, 0}, // 406 Class
					{1, 1, 1, 1, 1, 1, 0, 1, 1, 1}, // ASB
					{1, 1, 1, 1, 0, 1, 1, 0, 0, 0}, // Bus Stop
					{0, 0, 0, 0, 0, 0, 1, 1, 0, 1}, // A&W
					{0, 0, 0, 0, 0, 0, 1, 1, 1, 0}}; // Tim Hortons
// Node index[i][2]: i = node values. 0th index for i is x coordinate, next index is y coordinate. origin (0, 0) is top left
const int node_positions[10][2] = 	{{125, 300},
					{515, 160},
					{550, 310},
					{645, 410},
					{660, 260},
					{735, 160},
					{795, 410},
					{880, 230},
					{1075, 300},
					{1150, 300}};
// Names of the nodes
string node_names[10] = {"GYM",
			"ENSC 429",
			"ENSC 482",
			"LOCKERS",
			"MACKENZIE",
			"ENSC 406",
			"ASB",
			"BUS STOP",
			"A&W",
			"TIM HORTONS"};

/*
class MyDesk : public Fl_Group {
    // Draw mouse coords in small black rectangle
    void draw_coords() {
        // Coordinates as a string
        char s[80];
        sprintf(s, "x=%d y=%d", (int)Fl::event_x(), (int)Fl::event_y());
        // Black rect
        fl_color(FL_BLACK);
        fl_rectf(10,10,200,25);
        // White text
        fl_color(FL_WHITE);
        fl_font(FL_HELVETICA, 18);
        fl_draw(s, 15, 25);
    }
};
*/

// -1 = no node selected, 0 - 9 = node value (index) selected
int node_selected = -1;

// Function Declarations
void findNode(Fl_Widget *widget, void *);

// TODO add description of class
// TODO add when a node is selected, that its edges turn red (as well as the node)
class Graph : public Fl_Group {
	protected:
		void draw() {
			// Variables
			unsigned int node_index;
			unsigned int edge_index;

			// Draw for each node -> name, node rectangle, edges connecting to node
			for (node_index = 0; node_index < 10; node_index++) {
				// Draw node names
				fl_color(FL_BLACK);
				fl_font(FL_HELVETICA, 18);
				fl_draw(node_names[node_index].c_str(), x() + node_positions[node_index][0] - 15, y() + node_positions[node_index][1] - 15);
				// Blue rectangle (unless selected)
				if (node_selected == node_index) {	
					fl_color(FL_RED);
				}
				else {
					fl_color(FL_BLUE);
				}
				fl_color(FL_BLUE);
				fl_rectf(x() + node_positions[node_index][0], y() + node_positions[node_index][1], 25, 25);
				// Edge lines (blue unless selected)
				for (edge_index = 0; edge_index < 10; edge_index++) {
					// Only add an edge between nodes if it should exist
					if (node_connections[node_index][edge_index] == 1) {
						if (node_selected == node_index) {
							fl_color(FL_RED);
						}
						else {
							fl_color(FL_BLUE);
						}
						fl_line(x() + node_positions[node_index][0] + 12.5f, y() + node_positions[node_index][1] + 12.5f,
								x() + node_positions[edge_index][0] + 12.5f, y() + node_positions[edge_index][1] + 12.5f);
					}
				}
				// Black text (above rectangle)
				// TODO
			}
		}
	public:
		Graph(int x, int y, int w, int h, const char *l=0) : Fl_Group(x,y,w,h,l) {}
};

int main(int argc, char **argv) {
	
	// Variables;
	Fl_Window *main_window;
	Fl_Input *user_input;
	Fl_Button *find_node;
	Fl_PNG_Image *sfu_burnaby_campus;
	Fl_PNG_Image *resized_sfu_burnaby_campus;
	Fl_Box *main_screen;

	// Adds in support for various image types (I care only about .png)
	fl_register_images();

	// Create the main window	
	main_window = new Fl_Window(1200, 900, "Michael Chyziak - Homework 6");
		
	// Import the .png image of sfu burnaby (cropped) and resize to fit into 1200x600
	sfu_burnaby_campus = new Fl_PNG_Image("./Burnaby_campus_cropped.png");
	// Error
	if (!sfu_burnaby_campus) {
		return -1;
	}
	resized_sfu_burnaby_campus = (Fl_PNG_Image *) sfu_burnaby_campus->copy(1200, 600); // Preserving image ratio
	
	// Main window children
	main_window->begin();
		// Creates the user input and button to accept the input
		user_input = new Fl_Input(100, 50, 200, 50, "Node:"); // Child 0
		find_node = new Fl_Button(100, 150, 200, 50, "Find User Node"); // Child 1
		// Creates a box and places the image of sfu into the box
		main_screen = new Fl_Box(0, 300, 1200, 600); // Child 2
		main_screen->image(resized_sfu_burnaby_campus);
		// Draw graph's/edge's
		Graph graph(0, 300, 1200, 600);
	main_window->end();

	// Callback functions
	find_node->callback(findNode);
	
	// Show the main window and run fltk
	main_window->show(argc, argv);
	return Fl::run();
}

// TODO
void findNode(Fl_Widget *widget, void *) {
	
	// Variables
	Fl_Button *find_node=(Fl_Button*)widget;
	Fl_Input *user_input = (Fl_Input*) find_node->parent()->child(0);

	// TODO FINISH
	
	// TODO REMOVE DEBUG/TEST
	printf("%s\n", user_input->value());
}

/*
// FLTK (1.3) includes
#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <Fl/Fl_Window.H>

using namespace std;

int main(int agrc, char **argv) {



	// Create the main window
	Fl_Window main_window(1200, 900, "Michael Chyziak - Homework 6");

	// Set the burnaby campus image as background
	Fl_Box background(0, 0, 1200, 900);
	background.image(sfu_burnaby_campus);
	//background.resize(-200, 200, 1200, 900);

	// Show the window and run
	main_window.show();
	return Fl::run();
}
*/
/*
// Global Constants
// TODO

// Function Declarations
// TODO
void detectKeyboard(unsigned char key, int x, int y);
void displayGUI(int argc, char **argv);
void drawScreen();
void displayString(float x, float y, string input);

// Global Variables
// TODO

int main(int argc, char **argv) {

	// Variables
	int status;

	// Run GUI
	displayGUI(argc, argv);
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
	// TODO

        // Set Background Color to a light grey
        glClearColor(0.4, 0.4, 0.4, 1.0);

        // Clear screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Reset transformations
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
	
	// TODO DO the thing
	
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
*/
