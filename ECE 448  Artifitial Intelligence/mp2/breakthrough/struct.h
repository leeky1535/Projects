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
#include <cstdlib>
#include <algorithm>
#include <ctime>

using namespace std;


struct board_desc{

	int num_rows;
	int num_cols;
          //1 when player_X's turn, 0 when player_O's turn

};

struct piece{
	bool alive;
	int x;
	int y;
};

struct node{
	double bestValue;
	bool noValue;
	piece* O_pieces;
	piece* X_pieces;
	node* best_child;
};

struct player{

	public:
		char symbol;
		piece* pieces;
		int num_expanded_nodes;
		int num_moves;
		float time_used;
		int num_captured_opp;

		int prev_move;            //prev_move%3 = 0: move front-left, 1: move front, 2: move front-right from the player's view

		void (*search_type) (board_desc&, player&, player&, bool);
		int (*heuristics) (board_desc&, player&, player&);
};



int check_cell(piece* pieces, int col, int row){
  for(int i = 0; i < 16; i++){
    if(pieces[i].x == col && pieces[i].y == row && pieces[i].alive == true)
      return i;
  }
  return -1;
}
