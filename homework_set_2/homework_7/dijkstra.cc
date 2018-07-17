// Programmer: Michael Chyziak
// Date: Wednesday July 15, 2018
//
// Program description:
// 	10 distributed locations in my home neighbourhood as seen through google maps. 
// 	Each location is a node with places I travel to/from connected by an edge.
// 	If the user inputs a start and end node it will use dijkstra's algorithm to find the quickest path (highlighted in red).
// 	Weights of edges are estimated times it takes to walk from one node to another
// Required supplementary files:
// 	./google_map.png
// Got SFU burnaby campus map from here (un-cropped): 
// 	http://www.sfu.ca/content/dam/sfu/fs/files/Campus%20maps/Burnaby%20aerial%20map.pdf
// Borrowed some source code from:
// 	https://www.geeksforgeeks.org/printing-paths-dijkstras-shortest-path-algorithm/

// Standard libraries
#include <iostream>
#include <string>
#include <stdlib.h>
#include <cstring>
#include <math.h>
#include <stdio.h>
#include <limits.h>

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

// Number of vertices in the graph
#define V 10

// Global constants
bool button_pressed = false;
bool initial_start = true;
const int number_of_nodes = 10;
// Node index [i][j]: i = node value. node j = 1 if i and j are connected by an edge, 0 if not
int node_connections[10][10] = 		{{ 0, 100,  50,   0,   0,   0,   0,   0,   0,   0}, // RBC
					{100,   0,  60,   0,   0, 360,   0,   0,   0,   0}, // COAST CAPITAL
					{ 50,  60,   0,   0, 300, 330,   0, 540,   0,   0}, // SHOPPERS DRUG MART
					{  0,   0,   0,   0,   0, 270, 480, 420,   0,   0}, // LOTUS
					{  0,   0, 300,   0,   0,  30, 120,  90,   0,   0}, // SOCCER FIELD
					{  0, 360, 330, 270,  30,   0,  75,  45,   0,   0}, // COMMUNITY CENTRE
					{  0,   0,   0, 480, 120,  75,   0,  30,   0,   0}, // FRIEND'S HOSUE
					{  0,   0, 540, 420,  90,  45,  30,   0, 600, 360}, // MY HOME
					{  0,   0,   0,   0,   0,   0,   0, 600,   0,   0}, // WHITE SPOT
					{  0,   0,   0,   0,   0,   0,   0, 360,   0,   0}}; // HASHI SUSHI
// Node index[i][2]: i = node values. 0th index for i is x coordinate, next index is y coordinate. origin (0, 0) is top left
const int node_positions[10][2] = 	{{100, 410},
					{175, 375},
					{190, 410},
					{200, 450},
					{450, 460},
					{500, 425},
					{570, 350},
					{675, 315},
					{900, 5},
					{900, 260}};
int node_dijkstra[10][10] = 	{{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
				{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
				{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
				{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
				{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
				{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
				{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
				{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
				{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
				{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
// Names of the nodes
string node_names[10] = {"RBC",
			"COAST CAPITAL",
			"SHOPPERS DRUG MART",
			"LOTUS",
			"SOCCER FIELD",
			"COMMUNITY CENTRE",
			"FRIEND'S HOUSE",
			"MY HOME",
			"WHITE SPOT",
			"HASHI SUSHI"};

// -1 = no node selected, 0 - 9 = node value (index) selected
int node_start = -1;
int node_end = -1;

// Function Declarations
void findNode(Fl_Widget *widget, void *);
void printPath(int parent[], int prev, int j);
int minDistance(int dist[], bool sptSet[]);
void dijkstra(int graph[V][V], int src, int dest);

// Displays the entire graph structure on the screen along with nodes and edges (with proper colours)
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
			fl_draw("NODE NAMES:", 750, 25);
			fl_line(745, 26, 870, 26);
			if (initial_start == true) {
				for (node_index = 0; node_index < 10; node_index++) {
					fl_color(FL_BLACK);
					fl_font(FL_HELVETICA, 16);
					fl_draw(node_names[node_index].c_str(), 750, 50 + (25 * node_index));
				}
			}

			if (button_pressed == true || initial_start == true) {
			// Draw for each node -> name, node rectangle, edges connecting to node
			for (node_index = 0; node_index < 10; node_index++) {
				node_name_length = node_names[node_index].length();
				// Blue rectangle (unless selected)
				if (node_start == node_index || node_end == node_index) {
					fl_color(FL_RED);
				}
				else {
					fl_color(FL_BLUE);
				}
				fl_rectf(x() + node_positions[node_index][0] - 2, y() + node_positions[node_index][1], node_name_length * 9, 25);
				// Edge lines (blue unless selected)
				for (edge_index = 0; edge_index < 10; edge_index++) {
					// Only add an edge between nodes if it should exist
					if (node_connections[node_index][edge_index] > 0 || node_connections[edge_index][node_index] > 0) {
						if (node_dijkstra[node_index][edge_index] == 1 || node_dijkstra[edge_index][node_index] == 1) {
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
			
			// Reset node_dijkstra
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 10; j++) {
					node_dijkstra[i][j] = 0;
				}
			}
		}
	public:
		Graph(int x, int y, int w, int h, const char *l=0) : Fl_Group(x,y,w,h,l) {}
		// Redraw
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
	Fl_Input *user_input_start;
	Fl_Input *user_input_end;
	Fl_Button *find_node;
	Fl_PNG_Image *sfu_burnaby_campus;
	Fl_PNG_Image *resized_sfu_burnaby_campus;
	Fl_Box *main_screen;

	// Adds in support for various image types (I care only about .png)
	fl_register_images();

	// Create the main window	
	main_window = new Fl_Window(1200, 900, "Michael Chyziak - Homework 7");
		
	// Import the .png image of sfu burnaby (cropped) and resize to fit into 1200x600
	sfu_burnaby_campus = new Fl_PNG_Image("./google_map.png");
	// Error
	if (!sfu_burnaby_campus) {
		return -1;
	}
	resized_sfu_burnaby_campus = (Fl_PNG_Image *) sfu_burnaby_campus->copy(1200, 600); // Preserving image ratio

	// Main window children
	main_window->begin();
		// Creates the user input and button to accept the input
		user_input_start = new Fl_Input(100, 50, 200, 50, "Start Node:"); // Child 0
		user_input_end = new Fl_Input(400, 50, 200, 50, "End Node:"); // Child 1
		find_node = new Fl_Button(250, 150, 200, 50, "Find Shortest Path"); // Child 2
		// Creates a box and places the image of sfu into the box
		main_screen = new Fl_Box(0, 300, 1200, 600); // Child 3
		main_screen->box(FL_FLAT_BOX);
		main_screen->image(resized_sfu_burnaby_campus);
		// Draw graph's/edge's
		Graph graph(0, 300, 1200, 600); // Child 4
	main_window->end();

	// Callback functions
	find_node->callback(findNode);
	
	// Show the main window and run fltk
	main_window->show(argc, argv);
	return Fl::run();
}

// Finds the start and end nodes and displays their quickest path using dijkstra's algorithm 
void findNode(Fl_Widget *widget, void *) {

	// Variables
	Fl_Button *find_node=(Fl_Button*)widget;
	Fl_Input *user_input_start = (Fl_Input*) find_node->parent()->child(0);
	Fl_Input *user_input_end = (Fl_Input*) find_node->parent()->child(1);
	Graph *graph = (Graph*) find_node->parent()->child(4);
	unsigned int node_index;
	bool valid_end_node = false;
	bool valid_start_node = false;

	// Check if user input was valid node name is given
	for (node_index = 0; node_index < 10; node_index++) {
		if (user_input_start->value() == node_names[node_index]) {
			node_start = node_index;
			valid_start_node = true;
		}
		if (user_input_end->value() == node_names[node_index]) {
			node_end = node_index;
			valid_end_node = true;
		}
	}

	// Notify user that input was invalid and to try again or run dijkstra's and redraw screen
	if (valid_start_node == false) {
		printf("Invalid start node name. Try again!\n");
	}
	else if (valid_end_node == false) {
		printf("Invalid end node name. Try again!\n");
	}
	else {
		button_pressed = true;
		dijkstra(node_connections, node_start, node_end);
		graph->redraw();
	}
}

// https://www.geeksforgeeks.org/printing-paths-dijkstras-shortest-path-algorithm/
// A utility function to find the vertex with minimum distance value, from the set of vertices not yet included in shortest path tree
int minDistance(int dist[], bool sptSet[]) {
     
	// Initialize min value
	int min = INT_MAX;
       	int min_index;
 
	for (int v = 0; v < V; v++) {
		if (sptSet[v] == false && dist[v] <= min) {
			min = dist[v], min_index = v;
		}
	}
	return min_index;
}

// https://www.geeksforgeeks.org/printing-paths-dijkstras-shortest-path-algorithm/
// Some changes made by myself
// Funtion that implements Dijkstra's single source shortest path algorithm for a graph represented using adjacency matrix representation
void dijkstra(int graph[V][V], int src, int dest) {
	
	// Variables
	int dist[V]; // Will hold the shortest distance from src to each node
	bool sptSet[V]; // sptSet[i] will true if vertex i is included / in shortest path tree or shortest distance from src to i is finalized
	int parent[V]; // Parent array to store shortest path tree

	// Initialize all distances as INFINITE and stpSet[] as false
	for (int i = 0; i < V; i++) {
		parent[i] = -1;
		//parent[0] = -1; // ERROR in code online
		dist[i] = INT_MAX;
		sptSet[i] = false;
	}

	// Set src to src distance as 0
	dist[src] = 0;

	// Find shortest path from srt to dest node
	for (int count = 0; count < V - 1; count++) {

		// Pick the minimum distance vertex from the set of vertices not yet processed
		// 'u' is always equal to src in first iteration
		int u = minDistance(dist, sptSet);

		// Mark the picked vertex as processed
		sptSet[u] = true;
		
		// Update dist value of the adjacent vertices of the picked vertex
		for (int v = 0; v < V; v++) {
			// Update dist[v] only if is not in sptSet, there is an edge from u to v, and total weight of path from src to v through u is smaller than current value of dist[v]
			if (!sptSet[v] && graph[u][v] && dist[u] + graph[u][v] < dist[v]) {
				parent[v] = u;
				dist[v] = dist[u] + graph[u][v];
			}
		}
	}

	// change the edges as done in djikstra's algorithm
	printPath(parent, src, dest);
}

// Function to print shortest path from source to j using parent array
// https://www.geeksforgeeks.org/printing-paths-dijkstras-shortest-path-algorithm/
void printPath(int parent[], int prev, int j) {

	// Base Case : If j is source
	if (parent[j] == -1) {
		return;
	}

	// Recursive call
	printPath(parent, 0, parent[j]);

	// Select nodes for quickest path
	node_dijkstra[parent[j]][j] = 1;
	node_dijkstra[j][parent[j]] = 1;
}
