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
bool button_pressed = false;
bool initial_start = true;
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
					{1050, 300},
					{1100, 300}};
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
			unsigned int node_name_length;
		
			// Draw node names at top of display
			fl_color(FL_BLACK);
			fl_font(FL_HELVETICA, 16);
			fl_draw("NODE NAMES:", 350, 25);
			fl_line(345, 26, 470, 26);
			if (initial_start == true) {
				for (node_index = 0; node_index < 10; node_index++) {
					fl_color(FL_BLACK);
					fl_font(FL_HELVETICA, 16);
					fl_draw(node_names[node_index].c_str(), 350, 50 + (25 * node_index));
				}
			}

			if (button_pressed == true || initial_start == true) {
			// Draw for each node -> name, node rectangle, edges connecting to node
			for (node_index = 0; node_index < 10; node_index++) {
				node_name_length = node_names[node_index].length();
				// Blue rectangle (unless selected)
				if (node_selected == node_index) {
					fl_color(FL_RED);
				}
				else {
					fl_color(FL_BLUE);
				}
				fl_rectf(x() + node_positions[node_index][0] - 2, y() + node_positions[node_index][1], node_name_length * 10, 25);
				// Edge lines (blue unless selected)
				for (edge_index = 0; edge_index < 10; edge_index++) {
					// Only add an edge between nodes if it should exist
					if (node_connections[node_index][edge_index] == 1 || node_connections[edge_index][node_index] == 1) {
						if (node_selected == node_index || node_selected == edge_index) {
							fl_color(FL_RED);
						}
						else {
							fl_color(FL_BLUE);
						}
						fl_line(x() + node_positions[node_index][0] + 12.5f, y() + node_positions[node_index][1] + 12.5f,
							x() + node_positions[edge_index][0] + 12.5f, y() + node_positions[edge_index][1] + 12.5f);
					}
				}
				// Draw node names inside rectangle
				fl_color(FL_WHITE);
				fl_font(FL_HELVETICA, 12);
				fl_draw(node_names[node_index].c_str(), x() + node_positions[node_index][0], y() + node_positions[node_index][1] + 18);
			}
			button_pressed = false;
			initial_start = false;
			}
		}
	public:
		Graph(int x, int y, int w, int h, const char *l=0) : Fl_Group(x,y,w,h,l) {}
		//TODO
		int handle(int e) {
			int ret = Fl_Group::handle(e);
			switch ( e ) {
				case FL_ENTER:
					ret = 1; // FL_ENTER: must return(1) to receive FL_MOVE
					break;
				case FL_MOVE: // FL_MOVE: mouse movement causes 'user damage' and redraw..
					damage(FL_DAMAGE_USER1);
					ret = 1; 
					break;
				}
			ret = 1;
			return(ret);
		}
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
		user_input = new Fl_Input(100, 50, 200, 50, "Node:"); // Child 0 TODO
		find_node = new Fl_Button(100, 150, 200, 50, "Find User Node"); // Child 1 TODO
		// Displays all of the possible user string options
		// TODO
		// Creates a box and places the image of sfu into the box
		main_screen = new Fl_Box(0, 300, 1200, 600); // Child 2
		main_screen->box(FL_FLAT_BOX);
		main_screen->image(resized_sfu_burnaby_campus);
		// Draw graph's/edge's
		Graph graph(0, 300, 1200, 600); // Child 3
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
	Graph *graph = (Graph*) find_node->parent()->child(3);
	unsigned int node_index;

	// Redraw with the selected node and edges highlighted if a valid node name is given
	for (node_index = 0; node_index < 10; node_index++) {
		if (user_input->value() == node_names[node_index]) {
			node_selected = node_index;
			button_pressed = true;
			graph->redraw();
		}
		else {
			// Do nothing
		}
	}

	// Notify user that input was invalid and to try again
	if (button_pressed == false) {
		printf("Invalid node name. Try again!\n");
	}
}
