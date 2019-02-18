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
#include <utility>
//#include "struct.h"

//#include "heuristics.h"

using namespace std;

int try_move_a_piece(int i, player& player1, player& player2);
int choose_a_move(board_desc& board, player& curr_player, player& opponent, int opp_index);
node* alpha_beta_pruning(node* curr, board_desc& board, player& player1, player& player2, int alpha, int beta, bool curr_is_max, int depth);
