#include <string>
#include <fstream>
#include <iostream>
#include <queue>
#include <cmath>
#include <unistd.h> //for sleep
#include <stack>
#include <vector>
#include <math.h>
#include <ctime>
#include <climits>
#include <algorithm>
#include "main.h"

using namespace std;

void print_solution(cell** cells, puzzle_desc& puzzle, float time_used){
		for(int i = 0; i < puzzle.num_rows; i++){
			for(int j = 0; j < puzzle.num_cols; j++){
				cout << cells[i][j].color;
			}
			cout << endl;
		}
		cout << "Number of assignments : " << puzzle.num_assignments << endl;
		cout << "Runtime in seconds: " << time_used << endl;
}


void handle_row(cell** cells, string curr_line, int line_number, puzzle_desc& puzzle){
	cells[line_number] = new cell[puzzle.num_cols];
	for(int i = 0; i < puzzle.num_cols; i++){
		char temp = curr_line[i];
		cells[line_number][i].x = i;
		cells[line_number][i].y = line_number;
		cells[line_number][i].color = temp;

		if(temp == '_'){
			cells[line_number][i].source_or_not = 0;
		}
		else{
			cells[line_number][i].source_or_not = 1;
			puzzle.source_cells.push_back(&cells[line_number][i]);
			bool new_color = 1;
			for(unsigned j = 0; j < puzzle.domains.size() ; ++j){
				if(puzzle.domains[j] == temp)
					new_color = 0;
			}
			if(new_color == 1)
				puzzle.domains.push_back(temp);
		}
	}
}

bool check_near_cells(cell** cells, cell* curr_cell, puzzle_desc& puzzle){
	int num_same_color = 0;
	int num_space = 0;
	int x = curr_cell->x;
	int y = curr_cell->y;
	char color = curr_cell->color;
	if(x-1 >= 0){
		if(cells[y][x-1].color == color){
			num_same_color++;
		}
		if(cells[y][x-1].color == '_'){
			num_space++;
		}
	}
	if(x+1 < puzzle.num_cols){
		if(cells[y][x+1].color == color){
			num_same_color++;
		}
		if(cells[y][x+1].color == '_'){
			num_space++;
		}
	}
	if(y-1 >= 0){
		if(cells[y-1][x].color == color){
			num_same_color++;
		}
		if(cells[y-1][x].color == '_'){
			num_space++;
		}
	}
	if(y+1 < puzzle.num_rows){
		if(cells[y+1][x].color == color){
			num_same_color++;
		}
		if(cells[y+1][x].color == '_'){
			num_space++;
		}
	}

	if(curr_cell->source_or_not == 1 && num_same_color <= 1){
		if(num_space >= 1 - num_same_color)
		return 1;
	}
	else if(curr_cell->source_or_not == 0 && num_same_color <=2){
		if(num_space >= 2 - num_same_color)
		return 1;

	}
	return 0;
}

bool consistent_constraint(cell** cells, cell* curr_cell, puzzle_desc& puzzle, char color){
	curr_cell->color = color;
	for(int i = 0; i < puzzle.num_rows; i++){
		for(int j = 0; j < puzzle.num_cols; j++){
			if(cells[i][j].color != '_'){
				if(check_near_cells(cells, &cells[i][j], puzzle) == 0){
					curr_cell->color = '_';
					return 0;
				}
			}
		}
	}
	curr_cell->color = '_';
	return 1;
}

cell* find_unassigned_var_dumb(cell** cells, puzzle_desc puzzle){
	for(int i = 0; i < puzzle.num_rows; i++){
		for(int j = 0; j < puzzle.num_cols; j++){
			if(cells[i][j].color == '_'){
				return &cells[i][j];
			}
		}
	}
	return NULL;
}

bool dumb_tracking(cell** cells, puzzle_desc& puzzle){
	cell *curr_cell = find_unassigned_var_dumb(cells, puzzle);
	if(curr_cell == NULL)
		return 1;

	for(unsigned i = 0; i < puzzle.domains.size(); i++){
		if(consistent_constraint(cells, curr_cell, puzzle, puzzle.domains[i]) == 1){
			curr_cell->color = puzzle.domains[i];
			puzzle.num_assignments++;

			bool result = dumb_tracking(cells, puzzle);
			if(result == 1)
				return result;
			curr_cell->color = '_';
		}
	}
	return 0;
}

void push_color_no_repeat(vector<char>& colors, char color){
	bool new_color = 1;
	for(unsigned j = 0; j < colors.size(); j++){
		if(color == colors[j])
			new_color = 0;
	}
	if(new_color == 1)
	  colors.push_back(color);
}

int reorder_adj_colors_first(cell** cells, cell* curr_cell, puzzle_desc& puzzle){
	int x = curr_cell->x;
	int y = curr_cell->y;
	int num_adjacent_colors = 0;

	if(x-1 >= 0 && cells[y][x-1].color != '_'){
	  push_color_no_repeat(curr_cell->adj_colors, cells[y][x-1].color);
		num_adjacent_colors++;
	}
	if(x+1 < puzzle.num_cols && cells[y][x+1].color != '_'){
		push_color_no_repeat(curr_cell->adj_colors, cells[y][x+1].color);
		num_adjacent_colors++;
	}
	if(y-1 >= 0 && cells[y-1][x].color != '_'){
		push_color_no_repeat(curr_cell->adj_colors, cells[y-1][x].color);
		num_adjacent_colors++;
	}
	if(y+1 < puzzle.num_rows && cells[y+1][x].color != '_'){
		push_color_no_repeat(curr_cell->adj_colors, cells[y+1][x].color);
		num_adjacent_colors++;
	}

	for(unsigned i = 0; i < puzzle.domains.size(); i++){
		push_color_no_repeat(curr_cell->adj_colors, puzzle.domains[i]);
	}
	return num_adjacent_colors;
}

cell* find_unassigned_var_smart(cell** cells, puzzle_desc puzzle){
	for(int i = 0; i < puzzle.num_rows; i++){
		for(int j = 0; j < puzzle.num_cols; j++){
			if(cells[i][j].color == '_'){
				int n = reorder_adj_colors_first(cells, &cells[i][j], puzzle);
				if(n > 0){
					return &cells[i][j];
				}
			}
		}
	}
	return NULL;
}

bool smart_tracking(cell** cells, puzzle_desc& puzzle){
	cell *curr_cell = find_unassigned_var_smart(cells, puzzle);
	if(curr_cell == NULL)
		return 1;

	for(unsigned i = 0; i < curr_cell->adj_colors.size(); i++){
		if(consistent_constraint(cells, curr_cell, puzzle, curr_cell->adj_colors[i]) == 1){
			curr_cell->color = curr_cell->adj_colors[i];
			puzzle.num_assignments++;

			bool result = smart_tracking(cells, puzzle);
			if(result == 1)
				return result;
			curr_cell->color = '_';
		}
	}
	return 0;
	return 0;
}

int main(int argc, char* argv[]){
	if (argc!=3) {
		cout<< "Error, usage: ./mp1 [tracking_type] [puzzle file name]" <<endl;
		return 1;
	}
	char mode = argv[1][0];
	string puzzle_file_name = argv[2];

	int num_rows = 0;

	ifstream puzzle_file (puzzle_file_name.c_str());

	if(!puzzle_file.is_open()) {
		cout << "Error opening file" <<endl;

		return 1;
	}

	cell** cells;
	puzzle_desc puzzle;
	puzzle.solved = 0;
	puzzle.num_assignments = 0;

	if(puzzle_file.is_open())
	{
		string curr_line;

		if(getline(puzzle_file, curr_line) == NULL) {
			cout << "Error: no lines read " << endl;
			return 1;
		}

		puzzle.num_cols = curr_line.length() - 1;
		cells = new cell*[14];
		handle_row(cells, curr_line, num_rows, puzzle);
		num_rows++;

		while(getline(puzzle_file, curr_line))
		{
			handle_row(cells, curr_line, num_rows, puzzle);
			num_rows++;
		}
	}
	puzzle.num_rows = num_rows;

	clock_t start_time = clock();
	clock_t end_time;
	switch (mode) {

		case 'd' :
			dumb_tracking(cells, puzzle);
			break;

		case 's' :
			smart_tracking(cells, puzzle);
			break;

		default:
			cout<<"invalid search flags, choose: d, s"<<endl;
			return 1;
	}
	end_time = clock();
	float time_used = (end_time-start_time)/double(CLOCKS_PER_SEC)*1000;

	print_solution(cells, puzzle, time_used);


}
