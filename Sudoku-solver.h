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

	void printSudoku(int sudoku_q[9][9]);							//print a given sudoku
	bool initialiseSudoku(int sudoku_q[9][9], SUDOKU_ANS_BOARD &sudoku_ans);		//initialse a sudoku from a given question board
	void disableRow(SUDOKU_ANS_BOARD &sudoku_ans, int n, int row);				//disable every instance of n in a given row
	void disableColumn(SUDOKU_ANS_BOARD &sudoku_ans, int n, int column);			//disable every instance of n in a given column
	void disableBox(SUDOKU_ANS_BOARD &sudoku_ans, int n, int x, int y);			//disable every instance of n in a given box
	void disablePos(SUDOKU_ANS_BOARD &sudoku_ans, int n, int x, int y);			//disable n for a given position
	void finalize(SUDOKU_ANS_BOARD &sudoku_ans, int sudoku_q[9][9], int n, int x, int y);	//finalize n for a given position
	int count(int sudoku_q[9][9]);								//count the number of determined answers in a given sudoku
	bool checkError(SUDOKU_ANS_BOARD &sudoku_ans, int sudoku_q[9][9]);			//check if something went wrong

	void checkRows(SUDOKU_ANS_BOARD &sudoku_ans, int sudoku_q[9][9], bool print_steps);	//check the rows of a given sudoku for a number only possible in one place in the row
	void checkColumns(SUDOKU_ANS_BOARD &sudoku_ans, int sudoku_q[9][9], bool print_steps);	//check the columns of a given sudoku for a number only possible in one place in the column
	void nakedSingle(SUDOKU_ANS_BOARD &sudoku_ans, int sudoku_q[9][9], bool print_steps);	//check all the positions in a sudoku for a number that is the sole possibility in the box
	void checkBox(SUDOKU_ANS_BOARD &sudoku_ans, int sudoku_q[9][9], bool print_steps);	//check each 3x3 box of a sudoku for a number who has only one spot
	void nakedPair(SUDOKU_ANS_BOARD &sudoku_ans, bool print_steps);				//look for a naked pair
	void pointingBoxRows(SUDOKU_ANS_BOARD &sudoku_ans, bool print_steps);			//check each 3x3 box of sudoku's rows for a number whose sole possiblities lie in that row
	void pointingBoxColumns(SUDOKU_ANS_BOARD &sudoku_ans, bool print_steps);		//check each 3x3 box of sudoku's columns for a number whose sole possiblities lie in that column
	void boxLineReduceRow(SUDOKU_ANS_BOARD &sudoku_ans, bool print_steps);			//check if numbers manifest themselves in 2 boxes such that in box 3 numbers can be eliminated
	void boxLineReduceColumn(SUDOKU_ANS_BOARD &sudoku_ans, bool print_steps);		//check if numbers manifest themselves in 2 boxes such that in box 3 numbers can be eliminated
	void hiddenPair(SUDOKU_ANS_BOARD &sudoku_ans, bool print_steps);			//search for hidden pairs TODO:Test if it truly works 
	void nakedTriple(SUDOKU_ANS_BOARD &sudoku_ans, bool print_steps);			//search for naked triples
	void hiddenTriple(SUDOKU_ANS_BOARD &sudoku_ans, bool print_steps);			//TODO:Search for hidden triples
	void xWing(SUDOKU_ANS_BOARD &sudoku_ans, bool print_steps);				//TODO:Apply the x-wing algorithm

	//NOTE - the functions that solve a sudoku are sorted in the order of complexity and speed. The lower a function is, the lesser it should be used!
}
