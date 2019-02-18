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
#include <cstdlib>
#include <ctime>
#include "struct.h"

using namespace std;


int count_dead(player& player){
	int num = 0;
	for(int i = 0; i < 16; i++){
		if(player.pieces[i].alive == false)
		num++;
	}
	cout << endl;
	return num;
}


int game_done(player& player_O, player& player_X){
	if(player_O.num_captured_opp == 16)
		return 1;
	if(player_X.num_captured_opp == 16)
		return 2;

	for(int i = 0; i < 16; i++){
		if(player_O.pieces[i].y == 0)
			return 1;
	}
	for(int i = 0; i < 16; i++){
		if(player_X.pieces[i].y == 7)
			return 2;
	}
	return 0;
}

void print_solution(board_desc& board, player& player_O, player& player_X, int winner){
		for(int i = 0; i < board.num_rows; i++){
			for(int j = 0; j < board.num_cols; j++){
				if(check_cell(player_O.pieces, j, i) != -1)
					cout << 'O';
				else if(check_cell(player_X.pieces, j, i) != -1)
					cout << 'X';
				else
					cout << '.';
			}
			cout << endl;
		}
		player_X.num_captured_opp = count_dead(player_O);
		player_O.num_captured_opp = count_dead(player_X);
		if(winner == 1)
			cout << "The winner is O" << endl;
		else
			cout << "The winner is X" << endl;

		cout << "O's stat   number of expanded nodes of O: " << player_O.num_expanded_nodes << endl;
		cout << "           number of moves by O: " << player_O.num_moves << endl;
		cout << "           average number of expanded nodes per move by O: " << player_O.num_expanded_nodes/player_O.num_moves << endl;
		cout << "           number of captured X by O:" << player_O.num_captured_opp << endl;
		cout << "           total time consumed in seconds: " << player_O.time_used << endl;
		cout << "           average time per move in seconds: " << player_O.time_used/player_O.num_moves << endl;

		cout << endl;
		cout << "X's stat   number of expanded nodes of X: " << player_X.num_expanded_nodes << endl;
		cout << "           number of moves by X: " << player_X.num_moves << endl;
		cout << "           average number of expanded nodes per move by X: " << player_X.num_expanded_nodes/player_X.num_moves << endl;
		cout << "           number of captured O by X:" << player_X.num_captured_opp << endl;
		cout << "           total time consumed in seconds: " << player_X.time_used << endl;
		cout << "           average time per move in seconds: " << player_X.time_used/player_X.num_moves << endl;

}


void handle_pieces(board_desc& board, player& player_O, player& player_X){
	int X_piece_index = 0;
	int O_piece_index = 0;

  int i, j;
	for(i = 0; i < 2; i++){
		for(j = 0; j < board.num_cols; j++){
			player_X.pieces[X_piece_index].x = j;
			player_X.pieces[X_piece_index].y = i;
			player_X.pieces[X_piece_index].alive = true;
			X_piece_index++;
		}
	}
	for(i = 6; i < 8; i++){
		for(j = 0; j < board.num_cols; j++){
			player_O.pieces[O_piece_index].x = j;
			player_O.pieces[O_piece_index].y = i;
			player_O.pieces[O_piece_index].alive = true;
			O_piece_index++;
		}
	}
}

void copy_pieces(piece* copy_to, piece* copy_from){
		for(int i = 0; i < 16; i++){
			copy_to[i].alive = copy_from[i].alive;
			copy_to[i].x = copy_from[i].x;
			copy_to[i].y = copy_from[i].y;
		}
}

bool check_if_can_move(int i, node* curr, bool curr_is_max, bool Os_turn){              //return -2 if cannot move; -1 if can move but didn't catch opponent; >0 index of the opponent caugh
	if((curr_is_max ^ Os_turn) == 0){
		if(curr->O_pieces[i/3].alive == true){
	    int index = i/3;
	    int x = curr->O_pieces[index].x;
	    int y = curr->O_pieces[index].y;
	    int dir = i%3;
	    if(dir == 0 && x-1 >= 0 && y-1 >= 0 && check_cell(curr->O_pieces, x-1, y-1) == -1){
	        return true;
	    }
	    else if(dir == 1 && y-1 >= 0 && check_cell(curr->O_pieces, x, y-1) == -1 && check_cell(curr->X_pieces, x, y-1) == -1){
					return true;
	    }
	    else if(dir == 2 && x+1 <= 7 && y-1 >= 0 && check_cell(curr->O_pieces, x+1, y-1) == -1){
	        return true;
	    }
	    else return false;
		}
		else return false;
	}
	else{
		if(curr->X_pieces[i/3].alive == true){
	    int index = i/3;
	    int x = curr->X_pieces[index].x;
	    int y = curr->X_pieces[index].y;
	    int dir = i%3;
			if(dir == 0 && x+1 <= 7 && y+1 <= 7 && check_cell(curr->X_pieces, x+1, y+1) == -1){
		    return true;
		  }
		  else if(dir == 1 && y+1 <= 7 && check_cell(curr->X_pieces, x, y+1) == -1 && check_cell(curr->O_pieces, x, y+1) == -1){
				return true;
		  }
		  else if(dir == 2 && x-1 >= 0 && y+1 <= 7 && check_cell(curr->X_pieces, x-1, y+1) == -1){
				return true;
		  }
		    else return false;
		  }
	  return false;
	}
}



bool take_move(int i, node* curr, bool curr_is_max, bool Os_turn, player& player_O, player& player_X){              //return -2 if cannot move; -1 if can move but didn't catch opponent; >0 index of the opponent caugh
	if((curr_is_max ^ Os_turn) == 0){
		if(curr->O_pieces[i/3].alive == true){
	    int index = i/3;
	    int x = curr->O_pieces[index].x;
	    int y = curr->O_pieces[index].y;
	    int dir = i%3;
	    if(dir == 0 && x-1 >= 0 && y-1 >= 0 && check_cell(curr->O_pieces, x-1, y-1) == -1){
	        curr->O_pieces[index].x = x-1;
					curr->O_pieces[index].y = y-1;
					int opp_index = check_cell(curr->X_pieces, x-1, y-1);
					if(opp_index != -1){
						curr->X_pieces[opp_index].alive = false;
					}
					return true;
	    }
	    else if(dir == 1 && y-1 >= 0 && check_cell(curr->O_pieces, x, y-1) == -1 && check_cell(curr->X_pieces, x, y-1) == -1){
					curr->O_pieces[index].x = x;
					curr->O_pieces[index].y = y-1;
					return true;
	    }
	    else if(dir == 2 && x+1 <= 7 && y-1 >= 0 && check_cell(curr->O_pieces, x+1, y-1) == -1){
					curr->O_pieces[index].x = x+1;
					curr->O_pieces[index].y = y-1;
					int opp_index = check_cell(curr->X_pieces, x+1, y-1);
					if(opp_index != -1){
						curr->X_pieces[opp_index].alive = false;
					}
	        return true;
	    }
	    else return false;
		}
		else return false;
	}
	else if((curr_is_max ^ Os_turn) != 0){
		if(curr->X_pieces[i/3].alive == true){
	    int index = i/3;
	    int x = curr->X_pieces[index].x;
	    int y = curr->X_pieces[index].y;
	    int dir = i%3;
			if(dir == 0 && x+1 <= 7 && y+1 <= 7 && check_cell(curr->X_pieces, x+1, y+1) == -1){
				curr->X_pieces[index].x = x+1;
				curr->X_pieces[index].y = y+1;
				int opp_index = check_cell(curr->O_pieces, x+1, y+1);
				if(opp_index != -1){
					curr->O_pieces[opp_index].alive = false;
				}
		    return true;
		  }
		  else if(dir == 1 && y+1 <= 7 && check_cell(curr->X_pieces, x, y+1) == -1 && check_cell(curr->O_pieces, x, y+1) == -1){
				curr->X_pieces[index].x = x;
				curr->X_pieces[index].y = y+1;
				return true;
		  }
		  else if(dir == 2 && x-1 >= 0 && y+1 <= 7 && check_cell(curr->X_pieces, x-1, y+1) == -1){
				curr->X_pieces[index].x = x-1;
				curr->X_pieces[index].y = y+1;
				int opp_index = check_cell(curr->O_pieces, x-1, y+1);
				if(opp_index != -1){
					curr->O_pieces[opp_index].alive = false;
				}
				return true;
		  }
		    else return false;
		 }
	  	return false;
		}
	return false;
}

node* minimax_search(node* curr, board_desc& board, player& player_O, player& player_X, bool curr_is_max, int depth, bool Os_turn) {
  if (depth == 3) {
		if(Os_turn == 1)
    curr->bestValue = player_O.heuristics(board, player_O, player_X);
		else
		curr->bestValue = player_X.heuristics(board, player_X, player_O);
    return curr;
  }
	node* result = NULL;
  for(int i = 0; i < 48; i++){
		if(check_if_can_move(i, curr, curr_is_max, Os_turn) == true){
			node* child = new node;
			if(Os_turn == true){
				player_O.num_expanded_nodes++;
			}
			else{
				player_X.num_expanded_nodes++;
			}
			child->noValue = true;
			child->O_pieces = new piece[16];
			child->X_pieces = new piece[16];
			copy_pieces(child->O_pieces, curr->O_pieces);
			copy_pieces(child->X_pieces, curr->X_pieces);
			take_move(i, child, curr_is_max, Os_turn, player_O, player_X);
			minimax_search(child, board, player_O, player_X,!curr_is_max, depth+1, Os_turn);
			if(curr_is_max){
				if(curr->noValue){
					curr->bestValue = child->bestValue;
					curr->noValue = false;
					curr->best_child = child;
					result = curr;
				}
				else{
					curr->bestValue = max(curr->bestValue, child->bestValue);
					if(curr->bestValue == child->bestValue){
						curr->best_child = child;
						result = curr;
					}
				}
			}
			else{
				if(curr->noValue){
					curr->bestValue = child->bestValue;
					curr->noValue = false;
					curr->best_child = child;
					result = curr;
				}
				else{
					curr->bestValue = min(curr->bestValue, child->bestValue);
					if(curr->bestValue == child->bestValue){
						curr->best_child = child;
						result = curr;
					}
				}
			}

		}
	}
  return result;
}

void minimax(board_desc& board, player& player_O, player& player_X, bool Os_turn){
	node* root = new node;
	root->O_pieces = new piece[16];
	root->X_pieces = new piece[16];
	copy_pieces(root->O_pieces, player_O.pieces);
	copy_pieces(root->X_pieces, player_X.pieces);
	root->noValue = true;
	bool curr_is_max = true;
	int depth = 0;
	root->bestValue = 0;
	for(int i = 0; i < 48; i++){
		if(check_if_can_move(i, root, curr_is_max, Os_turn) == true){
			node* child = new node;
			child->O_pieces = new piece[16];
			child->X_pieces = new piece[16];
			copy_pieces(child->O_pieces, root->O_pieces);
			copy_pieces(child->X_pieces, root->X_pieces);
			take_move(i, child, curr_is_max, Os_turn, player_O, player_X);
			minimax_search(child, board, player_O, player_X,!curr_is_max, depth+1, Os_turn);
			if(root->noValue){
				root->bestValue = child->bestValue;
				root->noValue = false;
				root->best_child = child;
			}
			else{
				root->bestValue = max(root->bestValue, child->bestValue);
				if(root->bestValue == child->bestValue){
				  root->best_child = child;
				}
			}
		}
	}
	copy_pieces(player_O.pieces, root->best_child->O_pieces);
	copy_pieces(player_X.pieces, root->best_child->X_pieces);

}

node* alpha_beta_pruning(node* curr, board_desc& board, player& player_O, player& player_X, double alpha, double beta, bool curr_is_max, int depth, bool Os_turn) {
  if (depth == 5) {
		if(Os_turn == 1)
    curr->bestValue = player_O.heuristics(board, player_O, player_X);
		else
		curr->bestValue = player_X.heuristics(board, player_X, player_O);
    return curr;
  }
  if (curr_is_max)
    curr->bestValue = -9999999;
	else
		curr->bestValue = 9999999;
	node* result = NULL;
  for(int i = 0; i < 48; i++){
		if(check_if_can_move(i, curr, curr_is_max, Os_turn) == true){
			node* child = new node;
			if(Os_turn == true){
				player_O.num_expanded_nodes++;
			}
			else{
				player_X.num_expanded_nodes++;
			}
			child->O_pieces = new piece[16];
			child->X_pieces = new piece[16];
			child->best_child = NULL;
			copy_pieces(child->O_pieces, curr->O_pieces);
			copy_pieces(child->X_pieces, curr->X_pieces);
			take_move(i, child, curr_is_max, Os_turn, player_O, player_X);
			alpha_beta_pruning(child, board, player_O, player_X, alpha, beta, !curr_is_max, depth+1, Os_turn);
			if(curr_is_max){
				if(curr->bestValue <= child->bestValue){
					if(child->bestValue >= beta)
						return curr->best_child;
					curr->bestValue = child->bestValue;
					curr->best_child = child;
					result = curr;
					alpha = curr->bestValue;
				}
			}
			else{
				if(curr->bestValue >= child->bestValue){
					if(child->bestValue <= alpha)
						return curr->best_child;
					curr->bestValue = child->bestValue;
					curr->best_child = child;
					result = curr;
					beta = curr->bestValue;
				}
			}
		}
	}
  return result;
}

void alpha_beta(board_desc& board, player& player_O, player& player_X, bool Os_turn){
	node* root = new node;
	root->O_pieces = new piece[16];
	root->X_pieces = new piece[16];
	copy_pieces(root->O_pieces, player_O.pieces);
	copy_pieces(root->X_pieces, player_X.pieces);
	double alpha = -999999;
	double beta = +999999;
	bool curr_is_max = true;
	int depth = 0;
	root->bestValue = -110592;
	root->best_child = NULL;
	for(int i = 0; i < 48; i++){
		if(check_if_can_move(i, root, curr_is_max, Os_turn) == true){
			node* child = new node;
			child->O_pieces = new piece[16];
			child->X_pieces = new piece[16];
			copy_pieces(child->O_pieces, root->O_pieces);
			copy_pieces(child->X_pieces, root->X_pieces);
			take_move(i, child, curr_is_max, Os_turn, player_O, player_X);
			alpha_beta_pruning(child, board, player_O, player_X, alpha, beta, !curr_is_max, depth+1, Os_turn);
			root->bestValue = max(root->bestValue, child->bestValue);
			if(root->bestValue == child->bestValue){
				root->best_child = child;
				alpha = root->bestValue;
			}
			if(beta <= root->bestValue)
				break;
		}

	}
	copy_pieces(player_O.pieces, root->best_child->O_pieces);
	copy_pieces(player_X.pieces, root->best_child->X_pieces);
}

int offensive_1(board_desc& board, player& player1, player& player2){
	return 2*(30 - (16-player1.num_captured_opp)) + rand()%100;
}
int offensive_2(board_desc& board, player& player1, player& player2){
	int sum;
	for(int i = 0; i < 16; i++){
		if(player1.symbol == 'O'){
			sum += 7 - player1.pieces[i].y;
		}
		else{
			sum += player1.pieces[i].y;
		}
	}
	return 2*(sum) + rand()%100;
}
int defensive_1(board_desc& board, player& player1, player& player2){
	return 2*((16-player2.num_captured_opp)) + rand()%100;
}
int defensive_2(board_desc& board, player& player1, player& player2){
	int sum;
	for(int i = 0; i < 16; i++){
		if(player1.symbol == 'O'){
			sum += sqrt(player1.pieces[i].y);
		}
		else{
			sum += sqrt(7-player1.pieces[i].y);
		}
	}
	return 2*(sum) + rand()%100;
}


void start_game(board_desc& board, player& player_O, player& player_X){
	int winner = 0;
	clock_t start_turn;
	clock_t end_turn;
	while(game_done(player_O, player_X) == 0){
		start_turn = clock();
		player_O.search_type(board, player_O, player_X, true);
		end_turn = clock();
		player_O.time_used += (end_turn-start_turn)/double(CLOCKS_PER_SEC);
		player_O.num_moves++;
		if(game_done(player_O, player_X) != 0)
			break;
		start_turn = clock();
		player_X.search_type(board, player_O, player_X, false);
		end_turn = clock();
		player_X.time_used += (end_turn-start_turn)/double(CLOCKS_PER_SEC);
		player_X.num_moves++;
	}
	winner = game_done(player_O, player_X);
	print_solution(board, player_O, player_X, winner);

}


int main(int argc, char* argv[]){
	if (argc!=2) {
		cout<< "Error, usage: ./mp2 [game type number]" <<endl;
		cout<< "game type 1: Minimax (Offensive heuristics 1) vs Alpha-beta (Offensive heuristics 1)" << endl;
		cout<< "game type 2: Alpha-beta (Offensive heuristics 2) vs Alpha-beta (Defensive heuristics 1)" << endl;
		cout<< "game type 3: Alpha-beta (Defensive heuristics 2) vs Alpha-beta (Offensive heuristics 1)" << endl;
		cout<< "game type 4: Alpha-beta (Offensive heuristics 2) vs Alpha-beta (Offensive heuristics 1)" << endl;
		cout<< "game type 5: Alpha-beta (Defensive heuristics 2) vs Alpha-beta (Defensive heuristics 1)" << endl;
		cout<< "game type 6: Alpha-beta (Offensive heuristics 2) vs Alpha-beta (Defensive heuristics 2)" << endl;
		return 1;
	}
	char mode = argv[1][0];

	board_desc board;
	board.num_rows = 8;
	board.num_cols = 8;


	player player_X;
	player_X.symbol = 'X';
	player player_O;
	player_O.symbol = 'O';

	player_X.num_expanded_nodes = 0;
	player_X.num_moves = 0;
	player_X.time_used = 0;
	player_X.num_captured_opp = 0;
	player_X.pieces = new piece[16];

	player_O.num_expanded_nodes = 0;
	player_O.num_moves = 0;
	player_O.time_used = 0;
	player_O.num_captured_opp = 0;
	player_O.pieces = new piece[16];


	handle_pieces(board, player_O, player_X);
	srand(time(NULL));


	switch (mode) {

		case '1' :
			player_X.search_type = &minimax;
			player_X.heuristics = &offensive_1;
			player_O.search_type = &alpha_beta;
			player_O.heuristics = &defensive_1;

			break;

		case '2' :
			player_X.search_type = &alpha_beta;
			player_X.heuristics = &offensive_2;
			player_O.search_type = &alpha_beta;
			player_O.heuristics = &defensive_1;
			break;

		case '3' :
			player_X.search_type = &alpha_beta;
			player_X.heuristics = &defensive_2;
			player_O.search_type = &alpha_beta;
			player_O.heuristics = &offensive_1;
			break;

		case '4' :
			player_X.search_type = &alpha_beta;
			player_X.heuristics = &offensive_2;
			player_O.search_type = &alpha_beta;
			player_O.heuristics = &offensive_1;
			break;

		case '5' :
			player_X.search_type = &alpha_beta;
			player_X.heuristics = &defensive_2;
			player_O.search_type = &alpha_beta;
			player_O.heuristics = &defensive_1;
			break;

		case '6' :
			player_X.search_type = &alpha_beta;
			player_X.heuristics = &offensive_2;
			player_O.search_type = &alpha_beta;
			player_O.heuristics = &defensive_2;
			break;

		default:
			cout<<"invalid search flags, choose: d, s"<<endl;
			return 1;
	}
	start_game(board, player_O, player_X);



}
