#include "include/game_logic.h"
#include <iomanip>
//WARNING: DO NOT DELETE THE THREE LINES BELOW!
//WHEN YOU PUT YOUR CODE TO ACMOJ, YOU WOULD DELETE THE LINE 1 AND USE LINE 4-6 INSTEAD 
//#include "game_logic.h"
//#include "utils.h"
//#include "utils.cpp"

namespace PZ2048 {

static int row_num_ = 4, col_num_ = 4;
static int target_ = 0;
static uint game_seed_ = 0;
static bool undone_ = true;
static int steps_ = 0;

static int board_[10][10];
static int score_ = 0;
static int largest_ = 2;

static int board_last_[10][10];
static int board_temp_[10][10];
static int score_last_ = 0;
static int largest_last_ = 2;

void Start(int row_num, int col_num, int target, uint game_seed) {
    row_num_ = row_num, col_num_ = col_num;
    target_ = target;
    game_seed_ = game_seed;
    undone_ = true;

    for (int i = 0; i < row_num; i++)
        for (int j = 0; j < col_num; j++) {
            board_[i][j] = 0;
        }

    steps_ = 0;

    score_ = 0;
    largest_ = 2;

    PZ2048::srand(game_seed);
    TryGenerateTile();
}

std::pair<int, int> EndGame() {
    return std::make_pair(steps_, score_);
}

int GetRows() {
    return row_num_;
}

int GetCols() { 
    return col_num_;
}

bool TryRun(char dir) {
    int score_temp_ = score_;
    int largest_temp_ = largest_;
    for (int i = 0; i < row_num_; i++)
        for (int j = 0; j < col_num_; j++) {
            board_temp_[i][j] = board_[i][j];
        }

    switch (dir)
    {
    case 'w':
        for (int j = 0; j < col_num_; j++) {
            int pos = 0, value = 0, now;
            for (int i = 0; i < row_num_; i++) {
                now = board_[i][j];
                if (now == 0) continue;
                board_[i][j] = 0;
                if (value == 0) value = board_[pos][j] = now;
                else if (value == now) {
                        value = board_[pos][j] *= 2;
                        score_ += value;
                        if (value > largest_) {
                            largest_ = value;
                        }
                        value = 0;
                        ++pos;
                    }
                else {
                        ++pos;
                        value = board_[pos][j] = now;
                }
            }
        }
        break;

    case 'a':
        for (int i = 0; i < row_num_; ++i) {
            int pos = 0, value = 0, now;
            for (int j = 0; j < col_num_; ++j) {
                now = board_[i][j];
                if (now == 0) continue;
                board_[i][j] = 0;
                if (value == 0) {
                    value = board_[i][pos] = now;
                } else if (value == now) {
                    value = board_[i][pos] *= 2;
                    score_ += value;
                    if (value > largest_) {
                        largest_ = value;
                    }
                    value = 0;
                    ++pos;
                } else {
                    ++pos;
                    value = board_[i][pos] = now;
                }
            }
        }
        break;

    case 's':
        for (int j = 0; j < col_num_; ++j) {
            int pos = row_num_ - 1;
            int value = 0;
            for (int i = row_num_ - 1; i >= 0; --i) {
                int now = board_[i][j];
                if (now == 0) continue;
                board_[i][j] = 0;
                if (value == 0) {
                    value = board_[pos][j] = now;
                } else if (value == now) {
                    value = board_[pos][j] *= 2;
                    score_ += value;
                    if (value > largest_) {
                        largest_ = value;
                    }
                    value = 0;
                    --pos;
                } else {
                    --pos;
                    value = board_[pos][j] = now;
                }
            }
        }
        break;

    case 'd':
        for (int i = 0; i < row_num_; ++i) {
            int pos = col_num_ - 1;
            int value = 0;
            for (int j = col_num_ - 1; j >= 0; --j) {
                int now = board_[i][j];
                if (now == 0) continue;
                board_[i][j] = 0;
                if (value == 0) {
                    value = board_[i][pos] = now;
                } else if (value == now) {
                    value = board_[i][pos] *= 2;
                    score_ += value;
                    if (value > largest_) {
                        largest_ = value;
                    }
                    value = 0;
                    --pos;
                } else {
                    --pos;
                    value = board_[i][pos] = now;
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
            has_changed |= board_[i][j] != board_temp_[i][j];
        }
    }

    if (!has_changed) return false;

    for (int i = 0; i < row_num_; i++)
        for (int j = 0; j < col_num_; j++) {
            board_last_[i][j] = board_temp_[i][j];
        }
    undone_ = false;
    score_last_ = score_temp_;
    largest_last_ = largest_temp_;
    ++steps_;
    TryGenerateTile();
    return true;
}

bool Undo() {
    if (undone_) return false;

    undone_ = true;
    for (int i = 0; i < row_num_; i++)
        for (int j = 0; j < col_num_; j++) {
            board_[i][j] = board_last_[i][j];
        }

    score_ = score_last_;
    largest_ = largest_last_;
    --steps_;
    return true;
}

void SetTile(int row_index, int col_index, int value) {
    board_[row_index][col_index] = value;
    if (value > largest_) {
        largest_ = value;
    } 
}

int GetTile(int row_index, int col_index) {
    return board_[row_index][col_index];
}

int Score() { 
    return score_;
}

int Steps() { 
    return steps_;
}

int GetTarget() { 
    return target_;
}

bool HasReachedTarget() {
    return largest_ >= target_;
}

bool Stuck() {
    for (int i = 0; i < row_num_; i++)
        for (int j = 0; j < col_num_; j++) {
            if (board_[i][j] == 0) return false;
            if (i < row_num_ - 1 && board_[i][j] == board_[i+1][j]) return false;
            if (j < col_num_ - 1 && board_[i][j] == board_[i][j+1]) return false;
        }
    return true;
}

void PrintBoard() {
    std::cout << std::left;
    for (int i = 0; i < row_num_; ++i) {
        for (int j = 0; j < col_num_; ++j) {
            std::cout << std::setw(4) << board_[i][j];
            if (j + 1 < col_num_) std::cout << ' ';
        }
        std::cout << '\n';
    }
}

}
