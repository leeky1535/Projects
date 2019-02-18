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

using namespace std;

struct cell{

	public:
		char color;
		bool source_or_not;

		int x;
		int y;
		vector<char> adj_colors;
		cell() : source_or_not(0) {}
};

struct puzzle_desc{

	int num_rows;
	int num_cols;

	vector<char> domains;
	vector<cell*> source_cells;
	bool solved;
	unsigned num_assignments;
	double execution_time;
};
