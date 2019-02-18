#include <string>
#include <fstream>
#include <iostream>
#include <queue>
#include <unistd.h> //for sleep
#include <stack>
#include <vector>
#include <math.h>
#include <cmath>
#include <climits>
#include <algorithm>

#define WALL '%'
#define EMPTY ' '
#define START 'P'
#define DOT '.'
#define VISITED '~'

struct maze_desc{

	int num_rows;
	int num_cols;
	//
	cell* start;
	cell* goal;

	vector<cell*> goals;
};

struct cell{

	public:
		bool wall;
		bool visited;
		cell* previous;

		int x;
		int y;

		int manhattan_dist;
		cell* nearest_goal;
		int step_cost;
		int total_cost;
		int goal_order;

		cell() : visited(false), previous(NULL), manhattan_dist(0), step_cost(0), goal_order(-1) {}
};
