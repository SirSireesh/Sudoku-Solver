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

#pragma once

namespace CSudokuSolver
{
	struct SUDOKU_ANS {
		bool done = false;
		bool num[9] = {true, true, true, true, true, true, true, true, true};
	};
	struct SUDOKU_ANS_BOARD {
		bool changed;
		struct SUDOKU_ANS box[9][9];
	};

	//typedef SUDOKU_ANS_BOARD SUDOKU_ANS_BOARD;

	void print_sudoku(int sudoku_q[9][9]);							//print a given sudoku
	bool initialise_sudoku(int sudoku_q[9][9], SUDOKU_ANS_BOARD &sudoku_ans);		//initialse a sudoku
	void disable_row(SUDOKU_ANS_BOARD &sudoku_ans, int n, int row);				//disable every instance of n in a given row
	void disable_column(SUDOKU_ANS_BOARD &sudoku_ans, int n, int column);			//disable every instance of n in a given column
	void disable_box(SUDOKU_ANS_BOARD &sudoku_ans, int n, int x, int y);			//disable every instance of n in a given box
	void disable_pos(SUDOKU_ANS_BOARD &sudoku_ans, int n, int x, int y);			//disable n for a given position
	void finalize(SUDOKU_ANS_BOARD &sudoku_ans, int sudoku_q[9][9], int n, int x, int y);	//finalize n for a given position
	void check_rows(SUDOKU_ANS_BOARD &sudoku_ans, int sudoku_q[9][9]);			//check the rows of a given sudoku for a number only possible in one place in the row
	void check_columns(SUDOKU_ANS_BOARD &sudoku_ans, int sudoku_q[9][9]);			//check the columns of a given sudoku for a number only possible in one place in the column
	void naked_single(SUDOKU_ANS_BOARD &sudoku_ans, int sudoku_q[9][9]);			//check all the positions in a sudoku for a number that is the sole possibility in the box
	void naked_pair(SUDOKU_ANS_BOARD &sudoku_ans);						//look for a naked pair
	void check_box(SUDOKU_ANS_BOARD &sudoku_ans, int sudoku_q[9][9]);			//check each 3x3 box of a sudoku for a number who has only one spot
	void check_box_rows(SUDOKU_ANS_BOARD &sudoku_ans);					//check each 3x3 box of sudoku's rows for a number whose sole possiblities lie in that row
	void check_box_columns(SUDOKU_ANS_BOARD &sudoku_ans);					//check each 3x3 box of sudoku's columns for a number whose sole possiblities lie in that column
	void check_only_box_row(SUDOKU_ANS_BOARD &sudoku_ans);					//TODO:check if a number is only present within a given box for a given row
	void check_only_box_column(SUDOKU_ANS_BOARD &sudoku_ans);				//TODO:check if a number is only present within a given box for a given column
	void check_other_box_rows(SUDOKU_ANS_BOARD &sudoku_ans);				//TODO:check if numbers manifest themselves in 2 boxes such that in box 3 numbers can be eliminated
	void check_other_box_columns(SUDOKU_ANS_BOARD &sudoku_ans);				//TODO:check if numbers manifest themselves in 2 boxes such that in box 3 numbers can be eliminated
	void check_hidden(SUDOKU_ANS_BOARD &sudoku_ans);					//TODO:search for hidden pairs in a box
	void x_wing(SUDOKU_ANS_BOARD &sudoku_ans);						//TODO:Apply the very advanced and complicated x-wing algorithm
	int count(int sudoku_q[9][9]);								//count the number of determined answers in a given sudoku

	//NOTE - the functions that solve a sudoku are sorted in the order of complexity and speed. The lower a function is, the lesser it should be used!
}
