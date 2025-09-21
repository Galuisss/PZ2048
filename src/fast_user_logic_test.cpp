#include <algorithm>
#include <random>
#include <iomanip>
#include <chrono>
#include <iostream>
#include <sstream>
#include <cmath>
#include "include/utils.h"
//WARNING: DO NOT DELETE THE THREE LINES BELOW!
//WHEN YOU PUT YOUR CODE TO ACMOJ, YOU WOULD DELETE THE LINE 1 AND USE LINE 4-6 INSTEAD 
//#include "game_logic.h"
//#include "utils.h"
//#include "utils.cpp"

using namespace PZ2048;

static int row_num_ = 4, col_num_ = 4;
static int target_ = 0;
static uint game_seed_ = 0;
static bool undone_ = true;
static int steps_ = 0;

static int board_[10][10];
static int score_ = 0;
static int largest_ = 2;

static int board_temp_[10][10];

inline std::pair<std::pair<int, int>, int> TryGenerateTile() {
    std::vector<std::pair<int, int>> slot_list;
    for(int i = 0; i < row_num_; i++)
    for(int j = 0; j < col_num_; j++)
        if(board_[i][j] == 0)
            slot_list.emplace_back(i, j);
        if(slot_list.empty())
            return {{-1, -1}, -1};
    auto pos = PZ2048::rand(), sig = PZ2048::rand();
    pos %= slot_list.size();
    int row = slot_list[pos].first, col = slot_list[pos].second;
    sig %= 10;
    int val = (sig == 0 ? 4 : 2);
    board_[row][col] = val;
    return {{row, col}, val};
}

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
    ++steps_;
    TryGenerateTile();
    return true;
}

bool HasReachedTarget() {
    return largest_ >= target_;
}

bool Stuck() {
    for (int i = 0; i < row_num_; i++)
        for (int j = 0; j < col_num_; j++) {
            if (board_[i][j] == 0) return false;
            if (i < row_num_ - 1 && j < col_num_ - 1 && (board_[i][j] == board_[i][j+1] || board_[i][j] == board_[i+1][j])) return false;
        }
    return true;
}

char Decide() {
    static bool randed = false;
    static const char dir_map[4] = {'w', 's', 'a', 'd'};
    if(!randed) {
        PZ2048::srand(time(nullptr));
        randed = true;
    }

    static int dir = 0;
    dir = (dir + 1) % 4;
    return dir_map[dir];
}

int main() {
    auto start_time = std::chrono::steady_clock::now();
    int total_score = 0;
    int total_steps = 0;
    int games = 5000;
    target_ = 2048;
    row_num_ = 4, col_num_ = 4;
    for (int i = 0; i < games; ++i) {
        uint seed = i + 1;
        Start(row_num_, col_num_, target_,seed);
        while (true) {
            char oper = Decide();
            if (oper != 'w' && oper != 's' && oper != 'a' && oper != 'd') {
                continue;
            }
            TryRun(oper);
            if (HasReachedTarget()) {
                total_score += score_;
                total_steps += steps_;
                break;
            }
            if (Stuck()) {
                total_score += score_;
                total_steps += steps_;
                break;
            }
        }
    }
    auto end_time = std::chrono::steady_clock::now();
    double avg_score = total_score * 1.0 / games;
    double avg_steps = total_steps * 1.0 / games;
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    std::cout << "Average score over " << games << " games: " << avg_score << std::endl;
    std::cout << "Average steps over " << games << " games: " << avg_steps << std::endl;
    std::cout << "Total run time: " << ms << " ms" << std::endl;
    std::cout.flush();

    // 按公式输出分数
    int final_score = static_cast<int>(avg_score + 0.5);
    int output_pts = 0;
    if (final_score < 1000) {
        output_pts = 0;
    } else if (final_score < 3000) {
        output_pts = static_cast<int>(std::ceil(final_score * final_score / 200000.0)) + 30;
    } else if (final_score < 8000) {
        output_pts = static_cast<int>(std::ceil(12.27 * std::log2(final_score / 1000.0) + 55.55));
    } else {
        output_pts = std::min(static_cast<int>(std::ceil(42.92 * std::sqrt(std::log2(final_score / 1000.0)) + 18.16)), 110);
    }
    std::cout << "Score by formula: " << output_pts << std::endl;
    std::cout.flush();
    return 0;
}


