#include <string>
#include <fstream>
#include <iostream>
#include <queue>
#include <cmath>
#include <unistd.h> //for sleep
#include <stack>
#include <vector>
#include <math.h>
#include <cmath>
#include <climits>
#include <algorithm>
//referenced: cplusplus.com for use of libraries for...
//fstream, iostream, sleep in unistd.h, stack, queue, priority queue, math

using namespace std;


struct cell{

	public:
		char state;
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

struct greedy_manhatton
{
    bool operator() ( const cell* lhs, const cell* rhs ) const
    {
    		return lhs->manhattan_dist >= rhs->manhattan_dist;
    }
};

struct astar_manhatton
{
    bool operator() ( const cell* lhs, const cell* rhs ) const
    {
    		return lhs->manhattan_dist+lhs->step_cost >= rhs->manhattan_dist+rhs->step_cost;
    }
};
struct GoalGreater
{
    bool operator() ( const cell* lhs, const cell* rhs ) const
    {
        return lhs->goal_order < rhs->goal_order;
    }
} goalgreater;


struct maze_desc{

	int num_rows;
	int num_cols;
	//
	cell* start;

	vector<cell*> goals;
};


int calc_distance(cell* cell_1, cell* cell_2)
{
	int x_diff = (abs(cell_1->x - cell_2->x));
	int y_diff = (abs(cell_1->y - cell_2->y));
	return x_diff + y_diff;
}

/*int find_closest_goal(cell* curr, maze_desc& maze){
	unsigned i, index;
	int min_dist = maze.num_rows + maze.num_cols;
	cell* closest;
	if(maze.goals.empty())
		return -1;

	for(i = 0; i < maze.goals.size(); i++){
		int temp = calc_distance(curr, maze.goals[i]);
		if(temp <= min_dist){
			min_dist = temp;
			closest = maze.goals[i];
			index = i;
		}
	}
	maze.goal_number = maze.goal_number + 1;
	maze.goals.erase(maze.goals.begin()+index);
	maze.goal = closest;
	return 0;
}*/

void clean(cell** cells, maze_desc& maze)
{
	for(int i = maze.num_rows-1; i>=0; i--)
	{
		delete[] cells[i];
	}
	delete[] cells;
	return;
}

bool SolutionPath(cell* curr, maze_desc& maze) {
	cell* c = maze.goals[0];
	while(c->previous != NULL) {
		if (curr == c)
			return true;
		c = c->previous;
	}
	return false;
}

void print_solution(cell** cells, maze_desc& maze, int offset) {
	cout << "Solution: " << endl;

	if(!(maze.goals[0]->visited)){
		cout << "Could not find path from starting point to finish point!" << endl;
		return;
	}

	cell* current_cell = maze.goals[0];

	vector<cell*> path;
	int path_length = 0;

	while(current_cell != NULL && current_cell!=maze.start){
		path.push_back(current_cell);
		++path_length;
		current_cell = current_cell->previous;
	}

	for(int i=0; i<maze.num_rows; i++){
		for (int j=0; j<maze.num_cols; j++){
			if(i==maze.start->y && j==maze.start->x){
				cout << 'P';
				continue;
			}

			bool continue_flag = false;
			for(unsigned int k=0; k<maze.goals.size(); k++){
				if(i==maze.goals[k]->y && j==maze.goals[k]->x){
					if (cells[i][j].visited){
						cout << '@';
					}
					else
						cout << 'G';
					continue_flag=true;
					break;
				}
			}
			if (continue_flag)
				continue;

			cout << ((cells[i][j].state == '%')? '%':(cells[i][j].visited? (SolutionPath(&(cells[i][j]),maze)?'+':'.'):' ' )) ;
		}
		cout << endl;
	}
	cout << endl;
	cout << endl;
	cout << "path cost of the solution " << path_length + offset << endl;
}

void print_solution(cell** cells, maze_desc& maze) {
	maze.goals[0]->visited = true;
	print_solution(cells, maze, 0);

}


void handle_row(cell** cells, string curr_line, int line_number, maze_desc& maze)
{
	cells[line_number] = new cell[curr_line.length()];
	for(unsigned i = 0; i < curr_line.length(); i++){
		char temp = curr_line[i];
		cells[line_number][i].x = i;
		cells[line_number][i].y = line_number;
		cells[line_number][i].state = temp;

		if(temp == 'P'){
			maze.start = &cells[line_number][i];
		}
		else if(temp == '.'){
			maze.goals.push_back(&cells[line_number][i]);
		}
	}
}


void set_manhattan_distances(cell** cells, maze_desc& maze)
{
	for(int i=0; i<maze.num_rows; i++){
		for(int j=0; j<maze.num_cols; j++){
			int x_diff = (abs((maze.goals[0])->x - j));
			int y_diff = (abs((maze.goals[0])->y - i));
			cells[i][j].manhattan_dist = x_diff + y_diff;
		}
	}
}


void bfs(cell** cells, maze_desc& maze) {

	cell* curr;

	queue<cell*> bfs_queue;
	bfs_queue.push(maze.start);

	int expansions = 0;
	//int flag_start = 1;

	while(!bfs_queue.empty()) {
		curr = bfs_queue.front();
		curr->visited = 1;
		bfs_queue.pop();

		/*if (curr == maze.goal && curr != NULL) {
			flag_start = 1;
		}

		if(flag_start == 1){
			flag_start = 0;
			int a = find_closest_goal(curr, maze);
			if(a == -1){
				break;
			}
		}*/
		expansions++;
		if(curr == maze.goals[0])
			break;
		int curr_x = curr->x;
		int curr_y = curr->y;
		if(cells[curr_y-1][curr_x].state != '%' && cells[curr_y-1][curr_x].visited == 0){
			bfs_queue.push(&cells[curr_y-1][curr_x]);
			cells[curr_y-1][curr_x].previous = &cells[curr_y][curr_x];
			cells[curr_y-1][curr_x].visited = 1;
		}
		if(cells[curr_y+1][curr_x].state != '%' && cells[curr_y+1][curr_x].visited == 0){
			bfs_queue.push(&cells[curr_y+1][curr_x]);
			cells[curr_y+1][curr_x].previous = &cells[curr_y][curr_x];
		  cells[curr_y+1][curr_x].visited = 1;
		}
		if(cells[curr_y][curr_x+1].state != '%' && cells[curr_y][curr_x+1].visited == 0){
			bfs_queue.push(&cells[curr_y][curr_x+1]);
			cells[curr_y][curr_x+1].previous = &cells[curr_y][curr_x];
			cells[curr_y][curr_x+1].visited = 1;
		}
		if(cells[curr_y][curr_x-1].state != '%' && cells[curr_y][curr_x-1].visited == 0){
			bfs_queue.push(&cells[curr_y][curr_x-1]);
			cells[curr_y][curr_x-1].previous = &cells[curr_y][curr_x];
			cells[curr_y][curr_x-1].visited = 1;
		}

	}

	print_solution(cells, maze, 0);
	cout<< "number of nodes expanded:" << expansions << endl;

	//memory cleanup
	clean(cells, maze);

}

int dfs_recursion(cell** cells, cell* curr, maze_desc& maze, int& expansions){

	curr->visited = 1;
	expansions++;

	if (curr == maze.goals[0]) {
		return 1;
	}

	int curr_x = curr->x;
	int curr_y = curr->y;
	if(cells[curr_y-1][curr_x].state != '%' && cells[curr_y-1][curr_x].visited == 0){
		cells[curr_y-1][curr_x].previous = curr;
		if(dfs_recursion(cells, &cells[curr_y-1][curr_x], maze, expansions) == 1)
		   return 1;
	}
	if(cells[curr_y+1][curr_x].state != '%' && cells[curr_y+1][curr_x].visited == 0){
		cells[curr_y+1][curr_x].previous = curr;
		if(dfs_recursion(cells, &cells[curr_y+1][curr_x], maze, expansions) == 1)
		   return 1;
	}
	if(cells[curr_y][curr_x+1].state != '%' && cells[curr_y][curr_x+1].visited == 0){
		cells[curr_y][curr_x+1].previous = curr;
		if(dfs_recursion(cells, &cells[curr_y][curr_x+1], maze, expansions) == 1)
		   return 1;
	}
	if(cells[curr_y][curr_x-1].state != '%' && cells[curr_y][curr_x-1].visited == 0){
		cells[curr_y][curr_x-1].previous = curr;
		if(dfs_recursion(cells, &cells[curr_y][curr_x-1], maze, expansions) == 1)
		   return 1;
	}

	return 0;
}


void dfs(cell** cells, maze_desc& maze) {

	cell* curr = maze.start;
	int expansions = 0;

	curr->visited = 1;
	expansions++;

	if (curr == maze.goals[0]) {
		return;
	}

	int curr_x = curr->x;
	int curr_y = curr->y;
	if(cells[curr_y-1][curr_x].state != '%' && cells[curr_y-1][curr_x].visited == 0){
		cells[curr_y-1][curr_x].previous = curr;
		dfs_recursion(cells, &cells[curr_y-1][curr_x], maze, expansions);
	}
	if(cells[curr_y+1][curr_x].state != '%' && cells[curr_y+1][curr_x].visited == 0){
		cells[curr_y+1][curr_x].previous = curr;
		dfs_recursion(cells, &cells[curr_y+1][curr_x], maze, expansions);
	}
	if(cells[curr_y][curr_x+1].state != '%' && cells[curr_y][curr_x+1].visited == 0){
		cells[curr_y][curr_x+1].previous = curr;
		dfs_recursion(cells, &cells[curr_y][curr_x+1], maze, expansions);
	}
	if(cells[curr_y][curr_x-1].state != '%' && cells[curr_y][curr_x-1].visited == 0){
		cells[curr_y][curr_x-1].previous = curr;
		dfs_recursion(cells, &cells[curr_y][curr_x-1], maze, expansions);
	}


	print_solution(cells, maze, 0);
	cout<< "number of nodes expanded:" << expansions << endl;
	//memory cleanup
  clean(cells, maze);

}

int greedy_recursion(cell** cells, cell* curr, maze_desc& maze, int& expansions, priority_queue<cell*, vector<cell*>, greedy_manhatton>& min_heap){
	expansions++;
	if (curr == maze.goals[0]) {
		return 1;
	}
	int curr_x = curr->x;
	int curr_y = curr->y;
	if(cells[curr_y-1][curr_x].state != '%' && cells[curr_y-1][curr_x].visited == 0){
		cells[curr_y-1][curr_x].previous = curr;
		min_heap.push(&cells[curr_y-1][curr_x]);
		cells[curr_y-1][curr_x].visited = 1;
	}
	if(cells[curr_y+1][curr_x].state != '%' && cells[curr_y+1][curr_x].visited == 0){
		cells[curr_y+1][curr_x].previous = curr;
		min_heap.push(&cells[curr_y+1][curr_x]);
		cells[curr_y+1][curr_x].visited = 1;
	}
	if(cells[curr_y][curr_x+1].state != '%' && cells[curr_y][curr_x+1].visited == 0){
		cells[curr_y][curr_x+1].previous = curr;
		min_heap.push(&cells[curr_y][curr_x+1]);
		cells[curr_y][curr_x+1].visited = 1;
	}
	if(cells[curr_y][curr_x-1].state != '%' && cells[curr_y][curr_x-1].visited == 0){
		cells[curr_y][curr_x-1].previous = curr;
		min_heap.push(&cells[curr_y][curr_x-1]);
		cells[curr_y][curr_x-1].visited = 1;
	}

	cell* next = min_heap.top();
	min_heap.pop();

	if(greedy_recursion(cells, next, maze, expansions, min_heap) == 1)
	 return 1;

	 return 0;
}


void greedy(cell** cells, maze_desc& maze){
	int expansions = 0;
	cell* curr = maze.start;
	curr->visited = 1;
	priority_queue <cell*, vector<cell*>, greedy_manhatton> min_heap;

	expansions++;
	int curr_x = curr->x;
	int curr_y = curr->y;
	if(cells[curr_y-1][curr_x].state != '%' && cells[curr_y-1][curr_x].visited == 0){
		cells[curr_y-1][curr_x].previous = curr;
		min_heap.push(&cells[curr_y-1][curr_x]);
		cells[curr_y-1][curr_x].visited = 1;
	}
	if(cells[curr_y+1][curr_x].state != '%' && cells[curr_y+1][curr_x].visited == 0){
		cells[curr_y+1][curr_x].previous = curr;
		min_heap.push(&cells[curr_y+1][curr_x]);
		cells[curr_y+1][curr_x].visited = 1;
	}
	if(cells[curr_y][curr_x+1].state != '%' && cells[curr_y][curr_x+1].visited == 0){
		cells[curr_y][curr_x+1].previous = curr;
		min_heap.push(&cells[curr_y][curr_x+1]);
		cells[curr_y][curr_x+1].visited = 1;
	}
	if(cells[curr_y][curr_x-1].state != '%' && cells[curr_y][curr_x-1].visited == 0){
		cells[curr_y][curr_x-1].previous = curr;
		min_heap.push(&cells[curr_y][curr_x-1]);
		cells[curr_y][curr_x-1].visited = 1;
	}

	cell* next = min_heap.top();
	min_heap.pop();
	greedy_recursion(cells, next, maze, expansions, min_heap);

	print_solution(cells, maze, 0);
	cout<< "number of nodes expanded:" << expansions << endl;
	//memory cleanup
	clean(cells, maze);
}

int astar_recursion(cell** cells, cell* curr, maze_desc& maze, int& expansions, priority_queue<cell*, vector<cell*>, astar_manhatton>& min_heap){
	expansions++;
	if (curr == maze.goals[0]) {
		return 1;
	}
	int curr_x = curr->x;
	int curr_y = curr->y;
	if(cells[curr_y-1][curr_x].state != '%' && cells[curr_y-1][curr_x].visited == 0){
		cells[curr_y-1][curr_x].previous = curr;
		cells[curr_y-1][curr_x].step_cost = curr->step_cost + 1;
		min_heap.push(&cells[curr_y-1][curr_x]);
		cells[curr_y-1][curr_x].visited = 1;
	}
	if(cells[curr_y+1][curr_x].state != '%' && cells[curr_y+1][curr_x].visited == 0){
		cells[curr_y+1][curr_x].previous = curr;
		cells[curr_y+1][curr_x].step_cost = curr->step_cost + 1;
		min_heap.push(&cells[curr_y+1][curr_x]);
		cells[curr_y+1][curr_x].visited = 1;
	}
	if(cells[curr_y][curr_x+1].state != '%' && cells[curr_y][curr_x+1].visited == 0){
		cells[curr_y][curr_x+1].previous = curr;
		cells[curr_y][curr_x+1].step_cost = curr->step_cost + 1;
		min_heap.push(&cells[curr_y][curr_x+1]);
		cells[curr_y][curr_x+1].visited = 1;
	}
	if(cells[curr_y][curr_x-1].state != '%' && cells[curr_y][curr_x-1].visited == 0){
		cells[curr_y][curr_x-1].previous = curr;
		cells[curr_y][curr_x-1].step_cost = curr->step_cost + 1;
		min_heap.push(&cells[curr_y][curr_x-1]);
		cells[curr_y][curr_x-1].visited = 1;
	}

	cell* next = min_heap.top();
	min_heap.pop();

	if(astar_recursion(cells, next, maze, expansions, min_heap) == 1)
	 return 1;

	 return 0;
}


void astar(cell** cells, maze_desc& maze){
	int expansions = 0;
	cell* curr = maze.start;
	curr->step_cost = 0;
	curr->visited = 1;
	priority_queue <cell*, vector<cell*>, astar_manhatton> min_heap;

	expansions++;
	int curr_x = curr->x;
	int curr_y = curr->y;
	if(cells[curr_y-1][curr_x].state != '%' && cells[curr_y-1][curr_x].visited == 0){
		cells[curr_y-1][curr_x].previous = curr;
		cells[curr_y-1][curr_x].step_cost = curr->step_cost + 1;
		min_heap.push(&cells[curr_y-1][curr_x]);
		cells[curr_y-1][curr_x].visited = 1;
	}
	if(cells[curr_y+1][curr_x].state != '%' && cells[curr_y+1][curr_x].visited == 0){
		cells[curr_y+1][curr_x].previous = curr;
		cells[curr_y+1][curr_x].step_cost = curr->step_cost + 1;
		min_heap.push(&cells[curr_y+1][curr_x]);
		cells[curr_y+1][curr_x].visited = 1;
	}
	if(cells[curr_y][curr_x+1].state != '%' && cells[curr_y][curr_x+1].visited == 0){
		cells[curr_y][curr_x+1].previous = curr;
		cells[curr_y][curr_x+1].step_cost = curr->step_cost + 1;
		min_heap.push(&cells[curr_y][curr_x+1]);
		cells[curr_y][curr_x+1].visited = 1;
	}
	if(cells[curr_y][curr_x-1].state != '%' && cells[curr_y][curr_x-1].visited == 0){
		cells[curr_y][curr_x-1].previous = curr;
		cells[curr_y][curr_x-1].step_cost = curr->step_cost + 1;
		min_heap.push(&cells[curr_y][curr_x-1]);
		cells[curr_y][curr_x-1].visited = 1;
	}

	cell* next = min_heap.top();
	min_heap.pop();
	astar_recursion(cells, next, maze, expansions, min_heap);

	print_solution(cells, maze, 0);
	cout<< "number of nodes expanded:" << expansions << endl;
	//memory cleanup
	clean(cells, maze);
}

int main(int argc, char* argv[])
{

	if (argc!=3) {
		cout<< "Error, usage: ./mp1 [search flag] [maze file name]" <<endl;
		return 1;
	}

	char mode = argv[1][0];
	string maze_file_name = argv[2];

	int num_cols = 0;
	int num_rows = 0;


	ifstream maze_file (maze_file_name.c_str());

	if(!maze_file.is_open()) {
		cout << "Error opening file" <<endl;

		return 1;
	}

	cell** cells;
	maze_desc maze;

	if(maze_file.is_open())
	{
		string curr_line;

		if(getline(maze_file, curr_line) == NULL) {
			cout << "Error: no lines read " << endl;
			return 1;
		}

		num_cols = curr_line.length();
		cells = new cell*[num_cols];
		handle_row(cells, curr_line, num_rows, maze);
		num_rows++;

		while(getline(maze_file, curr_line))
		{
			handle_row(cells, curr_line, num_rows, maze);
			num_rows++;
		}
	}
	maze.num_rows = num_rows;
	maze.num_cols = num_cols;


	switch (mode) {

		case 'b' :
			set_manhattan_distances(cells, maze);
			bfs(cells, maze);
			break;

		case 'd' :
			set_manhattan_distances(cells, maze);
			dfs(cells, maze);
			break;

		case 'g' :
			set_manhattan_distances(cells, maze);
			greedy(cells, maze);
			break;

		case 'a' :
			set_manhattan_distances(cells, maze);
			astar(cells, maze);
			break;

		default:
			cout<<"invalid search flags, choose: b, d, g, a"<<endl;
			return 1;
	}

	cout << "Start: " << "[" << maze.start->y << "," << maze.start->x <<"]"<< endl;
	cout << "Goal: " << "[" << maze.goals[0]->y << "," << maze.goals[0]->x << "]" << endl;


}
