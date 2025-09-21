#include <algorithm>
#include <random>
#include "include/user_logic.h"
#include "include/utils.h"
#include "include/game_logic.h"
//WARNING: DO NOT DELETE THE LINES BELOW!
//WHEN YOU PUT YOUR CODE TO ACMOJ, YOU WOULD DELETE THE LINE 3-5 AND USE LINE 8-12 INSTEAD 
//#include "game_logic.h"
//#include "game_logic.cpp"
//#include "utils.h"
//#include "utils.cpp"
//#include "user_logic.h"

namespace PZ2048 {

using std::find;

static int row_num_ = 4, col_num_ = 4;
static int board_[10][10];

void ClientPrepare(int r, int c) {
    row_num_ = r;
    col_num_ = c;
}

void ReadBoard() {
    for (int i = 0; i < row_num_; ++i) {
        for (int j = 0; j < col_num_; ++j) {
            std::cin >> board_[i][j];
        }
    }
}

char Decide() {
    static bool randed = false;
    static const char dir_map[4] = {'w', 's', 'a', 'd'};
    if(!randed) {
        srand(time(nullptr));
        randed = true;
    }

    static int dir = 0;
    dir = (dir + 1) % 4;
    return dir_map[dir];
}

}

