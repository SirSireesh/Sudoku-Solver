/*  This library can be used to solve a given sudoku by applying simple algorithms that mortals use to solve sudokus
    Copyright (C) 2016  Kiran Dhana, Sireesh Kodali

    Sudoku-solver is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Sudoku-solver is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    contact - 
    sireeshkodali1@gmail.com
    dkiran100@gmail.com
    */
#include <iostream>
#include "Sudoku-solver.h"

bool CSudokuSolver::initialise_sudoku(int sudoku_q[9][9], SUDOKU_ANS_BOARD &sudoku_ans)
{
	for (int i = 0; i < 9; ++i)
		for (int j = 0; j < 9; ++j) {
			if (sudoku_ans.box[i][j].done == true || (sudoku_q[i][j] && sudoku_ans.box[i][j].num[sudoku_q[i][j] - 1] == false)) 
				return false;
			if (sudoku_q[i][j])
				finalize(sudoku_ans, sudoku_q, sudoku_q[i][j] - 1, i, j);	//finalize expects numbers in terms of array subscripts, hence num - 1
		}
	return true;
}

void CSudokuSolver::print_sudoku(int sudoku_q[9][9])
{
	for (int i = 0; i < 9; ++i) {
		for (int j = 0; j < 9; ++j)
			!(sudoku_q[i][j]) ? std::cout << " |" : std::cout << sudoku_q[i][j] << '|';
		std::cout << '\n';
	}
}

int CSudokuSolver::count(int sudoku_q[9][9])
{
	int num_answered = 0;
	for (int i = 0; i < 9; ++i)
		for (int j = 0; j < 9; ++j) 
			if (sudoku_q[i][j])
				++num_answered;
	return num_answered;
}

void CSudokuSolver::disable_pos(SUDOKU_ANS_BOARD &sudoku_ans, int n, int x, int y)
{
	if (sudoku_ans.box[x][y].num[n] == true && sudoku_ans.box[x][y].done == false) {
		sudoku_ans.changed = true;
		sudoku_ans.box[x][y].num[n] = false;
	}
}

void CSudokuSolver::disable_column(SUDOKU_ANS_BOARD &sudoku_ans, int n, int column)
{
	for (int i = 0; i < 9; ++i) {
		if (sudoku_ans.box[i][column].done == false)
			disable_pos(sudoku_ans, n, i, column);
	}
}

void CSudokuSolver::disable_row(SUDOKU_ANS_BOARD &sudoku_ans, int n, int row)
{
	for (int j = 0; j < 9; ++j) {
		if (sudoku_ans.box[row][j].done == false)
			disable_pos(sudoku_ans, n, row, j);
	}

}

void CSudokuSolver::disable_box(SUDOKU_ANS_BOARD &sudoku_ans, int n, int x, int y)
{
	for (int i = x - (x%3); i < (x - (x%3) + 3); ++i) {
		for (int j = y - (y%3); j < (y  - (y%3) + 3); ++j) {
			if (sudoku_ans.box[i][j].done == false)
				disable_pos(sudoku_ans, n, i, j);
		}
	}
}

void CSudokuSolver::finalize(SUDOKU_ANS_BOARD &sudoku_ans, int sudoku_q[9][9], int n, int x, int y)
{
	if (sudoku_ans.box[x][y].num[n] == false) {
		std::cout << "error : The number " << n + 1 << " was set to false at " << x << ' ' << y << "!\n";
		return;
	}
	if (sudoku_ans.box[x][y].done) {
		std::cout << "error : The position " << x << ' ' << y << " was set to done!" << " Number " << sudoku_q[x][y] << " is already there!\n";
		return;
	}
	sudoku_ans.box[x][y].done = true;
	sudoku_ans.changed = true;
	sudoku_q[x][y] = n + 1;

	for (int i = 0; i < 9; ++i) {
		sudoku_ans.box[x][y].num[i] = false;
	}
	disable_row(sudoku_ans, n, x);
	disable_column(sudoku_ans, n, y);
	disable_box(sudoku_ans, n, x, y);
}

void CSudokuSolver::check_columns(SUDOKU_ANS_BOARD &sudoku_ans, int sudoku_q[9][9])
{
	/*
	 * The logic here is simple -
	 * go through each column
	 * if the position is the only possible position for a number in a column
	 * mark that as the actual position of the number
	 */

	//the bool keeps track of whether this is the only possible position of a number in a column or not
	bool only_pos[9];

	for (int i = 0; i < 9; ++i) {
		for (auto& k : only_pos) 	//reset the bool for every column we check
			k = true;
		for (int j = 0; j < 9; ++j) {
			if (sudoku_ans.box[j][i].done == false) 	//make sure the position was not already filled
				for (int n = 0; n < 9; ++n) {
					if (sudoku_ans.box[j][i].num[n] && only_pos[n]) {
						for (int k = j + 1; k < 9; ++k)
							if (sudoku_ans.box[k][i].done == false && sudoku_ans.box[k][i].num[n]) {
								only_pos[n] = false;
								break;
							}
						if (only_pos[n]) 		//if it is the only posiion, finalize it
							finalize(sudoku_ans, sudoku_q, n, j, i);
					}
				}
		}
	}
}

void CSudokuSolver::check_rows(SUDOKU_ANS_BOARD &sudoku_ans, int sudoku_q[9][9])
{
	/*
	 * The logic here is simple
	 * go through each row
	 * if the position us the only possible position in a row
	 * mark that as the actual position of the number
	 */

	//the bool keeps track of whether this is the only possible position of a number in a row or not
	bool only_pos[9];

	for (int i = 0; i < 9; ++i){
		for (auto& k : only_pos)		//reset the bool for every row we check
			k = true;
		for (int j = 0; j < 9; ++j) {
			if (sudoku_ans.box[i][j].done == false) 	//make sure that the position is not already filled
				for (int n  = 0; n < 9; ++n)
					if (sudoku_ans.box[i][j].num[n] && only_pos[n]) {
						for (int k = j + 1; k < 9; ++k)
							if (sudoku_ans.box[i][k].done == false && sudoku_ans.box[i][k].num[n]) {
								only_pos[n] = false;
								break;
							}
						if (only_pos[n]) {
							finalize(sudoku_ans, sudoku_q, n, i, j);
						}
					}
		}
	}
}

void CSudokuSolver::naked_single(SUDOKU_ANS_BOARD &sudoku_ans, int sudoku_q[9][9])
{
	int poss = 0;
	int num;
	for (int i = 0; i < 9; ++i)
		for (int j = 0; j < 9; ++j) {
			poss = 0;
			if (sudoku_ans.box[i][j].done == false)
				for (int n = 0; n < 9; ++n)
					if (sudoku_ans.box[i][j].num[n] == true) {
						++poss;
						num = n;
					}
			if (poss == 1)
				finalize(sudoku_ans, sudoku_q, num, i, j);
		}
}

void CSudokuSolver::naked_pair(SUDOKU_ANS_BOARD &sudoku_ans)
{
	int counter[9][9];
	bool pair = true;
	int num[2] = {-1, -1};
	for (int i = 0; i < 9; ++i) 
		for (int j = 0; j < 9; ++j) {
			counter[i][j] = 0;
			if (sudoku_ans.box[i][j].done == false)
				for (int n = 0; n < 9; ++n)
					if (sudoku_ans.box[i][j].num[n])
						++counter[i][j];
		}

	//for the row
	for (int i = 0; i < 9; ++i)
		for (int j = 0; j < 9; ++j) {
			pair = true;
			if (counter[i][j] == 2) {
				for (int k = j + 1; k < 9; ++k) {
					if (counter[i][k] == 2 && sudoku_ans.box[i][k].done == false && sudoku_ans.box[i][j].done == false) {
						for (int n = 0; n < 9; ++n) {
							if (sudoku_ans.box[i][j].num[n] != sudoku_ans.box[i][k].num[n]) {
								pair = false;
								break;
							}
							else if (sudoku_ans.box[i][j].num[n]) {
								if (num[0] == -1)
									num[0] = n;
								else
									num[1] = n;
							}
						}
						if (pair) {
							//while using disable_* may seem better, it will let us into an infinite loop
							for (int l = 0; l < j; ++l){
								disable_pos(sudoku_ans, num[0], i, l);
								disable_pos(sudoku_ans, num[1], i, l);
							}
							for (int l = j + 1; l < k; ++l) {
								disable_pos(sudoku_ans, num[0], i, l);
								disable_pos(sudoku_ans, num[1], i, l);
							}
							for (int l = k + 1; l < 9; ++l) {
								disable_pos(sudoku_ans, num[0], i, l);
								disable_pos(sudoku_ans, num[1], i, l);
							}

							if (j - (j % 3) == k - (k % 3)) 
								//disable the box
								for (int l = i - (i % 3); l < (i - (i % 3) + 3); ++l) 
									for (int m = j - (j % 3); l < j - (j % 3) + 3; ++l)
										if (l == i && (m == j || m == k)) {
											disable_pos(sudoku_ans, num[0], l, m);
											disable_pos(sudoku_ans, num[1], l, m);

										}
						}
					}
				}
			}

		}
}

void CSudokuSolver::check_box(SUDOKU_ANS_BOARD &sudoku_ans, int sudoku_q[9][9])
{
	/* Algorithm is yet again simple - but avoid using as it is very slow
	 * Go through a 3x3 box
	 * For every num you see, check if the num is there anywhere else in the box
	 * If not, finalize it
	 * Implementation looks lengthy, DON'T PANIC! We can shorten it (hopefully)
	 */

	bool only_pos[9];

	for (int i = 0; i < 9; i += 3) {
		for (int j = 0; j < 9; j += 3) {
			for (auto& a : only_pos)
				a = true;
			for (int k = i; k < i + 3; ++k) {
				for (int l = j; l < j + 3; ++l) {
					//This gets us each individual box in the 3x3 box
					if (sudoku_ans.box[k][l].done == false)
						for (int n = 0; n < 9; ++n) {
							if (sudoku_ans.box[k][l].num[n] && only_pos[n]) {
								for (int m = i; m < i + 3; ++m) 
									for (int o = j; o < j + 3; ++o) 
										if (sudoku_ans.box[m][o].done == false) {
											if (m == k && o == l)
												;
											else if (sudoku_ans.box[m][o].num[n]) {
												only_pos[n] = false;
												break;
											}
										}

								if (only_pos[n]) 
									finalize(sudoku_ans, sudoku_q, n, k, l);
							}
						}
				}
			}
		}
	}
}

void CSudokuSolver::check_box_rows(SUDOKU_ANS_BOARD &sudoku_ans)
{
	/*
	 * Algorithm is complex - avoid using unless absolutely necessary!
	 * For every 3x3 box, check if a number fits only in a particular row
	 * If true, disable the number for the rest of the row
	 */

	bool only_row[9];

	for (int i = 0; i < 9; i += 3)
		for (int j = 0; j < 9; j += 3) {
			for (auto& a : only_row)
				a = true;
			for (int k = i; k < i + 3; ++k)
				for (int l = j; l < j + 3; ++l) {
					if (sudoku_ans.box[k][l].done == false) {
						for (int n = 0; n < 9; ++n) {
							if (sudoku_ans.box[k][l].num[n] && only_row[n]) {
								//check all the rows for the same num
								for (int m = i; m < i + 3; ++m)
									for (int o = j; o < j + 3; ++o) {
										if (m != k)
											if (sudoku_ans.box[m][o].done == false && sudoku_ans.box[m][o].num[n]) {
												only_row[n] = false;
												break;
											}
									}
								if (only_row[n]) {
									//disable n for the rest of the row
									for (int m = 0; m < j - (j % 3); ++m) {
										disable_pos(sudoku_ans, n, k, m);
									}
									for (int m = j - (j % 3) + 3; m < 9; ++m) {
										disable_pos(sudoku_ans, n, k, m);
									}
								}
							}
						}
					}
				}
		}
}

void CSudokuSolver::check_box_columns(SUDOKU_ANS_BOARD &sudoku_ans)
{
	/*
	 * Algorithm is complex
	 * For every 3x3 box, check if a number fits only in a particular row
	 * If true, disable the number for the rest of the row
	 */
	bool only_column[9];

	for (int i = 0; i < 9; i += 3)
		for (int j = 0; j < 9; j += 3) {
			for (auto& a : only_column)
				a = true;
			for (int k = i; k < i + 3; ++k)
				for (int l = j; l < j + 3; ++l) {
					if (sudoku_ans.box[l][k].done == false) {
						for (int n = 0; n < 9; ++n) {
							if (sudoku_ans.box[l][k].num[n] && only_column[n]) {
								//check all the columns for the same num
								for (int m = i; m < i + 3; ++m)
									for (int o = j; o < j + 3; ++o) {
										if (o != j)
											if (sudoku_ans.box[o][m].done == false && sudoku_ans.box[o][m].num[n]) {
												only_column[n] = false;
												break;
											}
									}
								if (only_column[n]) {
									//disable n for the rest of the column
									for (int m = 0; m < i - (i%3); ++m) {
										disable_pos(sudoku_ans, n, m, l);
									}
									for (int m = i - (i%3) + 3; m < 9; ++m) {
										disable_pos(sudoku_ans, n, m, l);
									}
								}
							}
						}
					}
				}
		}
}
