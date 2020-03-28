#ifndef SUDOKU_H
#define SUDOKU_H
#include <stdbool.h>
const bool DEBUG_MODE = false;
enum { ROW=9, COL=9, N = 81, NEIGHBOR = 20 };
const int NUM = 9;

bool solve_sudoku_dancing_links(int unused);
bool solved();
#endif
