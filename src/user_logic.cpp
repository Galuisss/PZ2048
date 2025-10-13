#include <algorithm>
#include <random>
#include <iostream>
#include "include/user_logic.h"
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

static bool has_read = false;
static int row_num_ = 4, col_num_ = 4;
static int board_[4][4];

struct SimState {
    int b[4][4];
};

static inline SimState snapshot_global() {
    SimState s{};
    for (int i = 0; i < row_num_; ++i)
        for (int j = 0; j < col_num_; ++j)
            s.b[i][j] = board_[i][j];
    return s;
}

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


bool TryRun(SimState& s, char dir) {
    int board_temp[4][4];

    for (int i = 0; i < row_num_; i++)
        for (int j = 0; j < col_num_; j++) {
            board_temp[i][j] = s.b[i][j];
        }

    switch (dir)
    {
    case 'w':
        for (int j = 0; j < col_num_; j++) {
            int pos = 0, value = 0, now;
            for (int i = 0; i < row_num_; i++) {
                now = s.b[i][j];
                if (now == 0) continue;
                s.b[i][j] = 0;
                if (value == 0) value = s.b[pos][j] = now;
                else if (value == now) {
                        value = s.b[pos][j] *= 2;
                        value = 0;
                        ++pos;
                    }
                else {
                        ++pos;
                        value = s.b[pos][j] = now;
                }
            }
        }
        break;

    case 'a':
        for (int i = 0; i < row_num_; ++i) {
            int pos = 0, value = 0, now;
            for (int j = 0; j < col_num_; ++j) {
                now = s.b[i][j];
                if (now == 0) continue;
                s.b[i][j] = 0;
                if (value == 0) {
                    value = s.b[i][pos] = now;
                } else if (value == now) {
                    value = s.b[i][pos] *= 2;
                    value = 0;
                    ++pos;
                } else {
                    ++pos;
                    value = s.b[i][pos] = now;
                }
            }
        }
        break;

    case 's':
        for (int j = 0; j < col_num_; ++j) {
            int pos = row_num_ - 1;
            int value = 0;
            for (int i = row_num_ - 1; i >= 0; --i) {
                int now = s.b[i][j];
                if (now == 0) continue;
                s.b[i][j] = 0;
                if (value == 0) {
                    value = s.b[pos][j] = now;
                } else if (value == now) {
                    value = s.b[pos][j] *= 2;
                    value = 0;
                    --pos;
                } else {
                    --pos;
                    value = s.b[pos][j] = now;
                }
            }
        }
        break;

    case 'd':
        for (int i = 0; i < row_num_; ++i) {
            int pos = col_num_ - 1;
            int value = 0;
            for (int j = col_num_ - 1; j >= 0; --j) {
                int now = s.b[i][j];
                if (now == 0) continue;
                s.b[i][j] = 0;
                if (value == 0) {
                    value = s.b[i][pos] = now;
                } else if (value == now) {
                    value = s.b[i][pos] *= 2;
                    value = 0;
                    --pos;
                } else {
                    --pos;
                    value = s.b[i][pos] = now;
                }
            }
        }
        break;

    default:
        return false;
    }

    bool has_changed = false;
    for (int i = 0; i < row_num_; i++) {
        if (has_changed) break;
        for (int j = 0; j < col_num_; j++) {
            if (has_changed) break;
            has_changed |= s.b[i][j] != board_temp[i][j];
        }
    }

    if (!has_changed) return false;
    return true;
}

static inline SimState place_tile(const SimState& s, int r, int c, int val) {
    SimState t = s;
    t.b[r][c] = val;
    return t;
}

static inline int count_empty(const SimState& s) {
    int z = 0;
    for (int i = 0; i < row_num_; ++i)
        for (int j = 0; j < col_num_; ++j)
            z += (s.b[i][j] == 0);
    return z;
}

bool Stuck(const SimState& s) {
    for (int i = 0; i < row_num_; i++)
        for (int j = 0; j < col_num_; j++) {
            if (s.b[i][j] == 0) return false;
            if (i < row_num_ - 1 && s.b[i][j] == s.b[i+1][j]) return false;
            if (j < col_num_ - 1 && s.b[i][j] == s.b[i][j+1]) return false;
        }
    return true;
}


static const char MOVES[4] = {'w','a','s','d'};

bool exist_2048(const SimState &s) {
    for (int i = 0; i < col_num_; i++) {
        for (int j = 0; j < row_num_; j++) {
            if (s.b[i][j] == 2048) {
                return true;
            }
        }
    }
    return false;
}

bool exist_1024_pair(const SimState &s) {
    for (int i = 0; i < row_num_; i++)
        for (int j = 0; j < col_num_; j++) {
            if (s.b[i][j] == 0) return false;
            if (i < row_num_ - 1 && s.b[i][j] == 1024 && s.b[i+1][j] == 1024) return true;
            if (j < col_num_ - 1 && s.b[i][j] == 1024 && s.b[i][j+1] == 1024) return true;
        }
    return false;
}

// 互递归核心
static double eval_state(const SimState& s) {

    static constexpr double weights[4][4] = {
        {15, 14, 13, 12}, 
        {8, 9, 10, 11}, 
        {7, 6, 5, 4}, 
        {0, 1, 2, 3}
    };
    
/*     static constexpr double weights2[4][4] = {
        {64, 32, 16, 8}, 
        {32, 16, 8, 4}, 
        {16, 8, 4, 2}, 
        {8, 4, 2, 1}
    }; */

    double score = 0;
    for (int i = 0; i < row_num_; ++i)
    for (int j = 0; j < col_num_; ++j)
        if (s.b[i][j] > 0)
            score += s.b[i][j] * weights[i][j];

    return score;
}

static double max_value(const SimState& s, int depth);

static double expect_value(const SimState& s, int depth) {
    int empties = count_empty(s);
    if (empties == 0) return eval_state(s);

    double p_cell = 1.0 / empties;
    double val = 0.0;
    for (int i=0;i<row_num_;++i)
        for (int j=0;j<col_num_;++j)
            if (s.b[i][j]==0) {
                SimState s2 = place_tile(s,i,j,2);
                SimState s4 = place_tile(s,i,j,4);
                val += p_cell * (0.9 * max_value(s2, depth-1)
                               + 0.1 * max_value(s4, depth-1));
            }
    return val;
}

static double max_value(const SimState& s, int depth) {
    if (depth == 0 || Stuck(s)) return eval_state(s);

    double best = -1e100;
    bool moved = false;
    for (char a : MOVES) {
        SimState t = s;
        if (TryRun(t, a)) {
            moved = true;
            double v = expect_value(t, depth);
            if (v > best) best = v;
        }
    }
    return moved ? best : eval_state(s);

}

// 选出在根节点的最佳动作
static char max_value_root(int maxDepth){
    SimState root = snapshot_global();

    double best = -1e100;
    char best_act = 'a';

    for (char a : MOVES) {
        SimState t = root;
        if (!TryRun(t, a)) continue;
        double v = expect_value(t, maxDepth);
        if (v > best) { best = v; best_act = a; }
    }
    if (best != -1e100) {
        return best_act;
    }
    return best_act;
} 

char Decide() {
    //PrintBoard();
    const int MAX_DEPTH = 3;
    return max_value_root(MAX_DEPTH);
}
}

