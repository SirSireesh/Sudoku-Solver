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
		struct SUDOKU_ANS box[9][9];
	};
	struct SUDOKU {
		bool changed = true;
		int sudoku_q[9][9];
		int sudoku_a[9][9];
		SUDOKU_ANS_BOARD sudoku_ans;
	};
	struct POINT {
		int x;
		int y;
	};

	bool getSudoku(SUDOKU &sudoku);								//take a sudoku for stdin
	bool initialiseSudoku(SUDOKU &sudoku);							//initialse a sudoku from a given question board
	void printSudoku(int sudoku_q[9][9]);							//unformatted print a sudoku
	void printfSudoku(SUDOKU sudoku);							//formatted print a given sudoku and its repective answer board
	void disablePos(SUDOKU &sudoku, int n, int x, int y);					//disable n for a given position
	void disableRow(SUDOKU &sudoku, int n, int row);						//disable every instance of n in a given row
	void disableColumn(SUDOKU &sudoku, int n, int column);					//disable every instance of n in a given column
	void disableBox(SUDOKU &sudoku, int n, int x, int y);					//disable every instance of n in a given box
	void finalize(SUDOKU &sudoku, int n, int x, int y, bool init);				//finalize n for a given position
	int count(int sudoku_q[9][9]);								//count the number of determined answers in a given sudoku
	bool checkError(SUDOKU sudoku);								//check if something went wrong
	int numCommon(SUDOKU sudoku, POINT pos1, POINT pos2);					//return the number of common possibilities
	int getCommon(SUDOKU sudoku, POINT pos1, POINT pos2);					//return the common number between 2 positions

	void nakedSingle(SUDOKU &sudoku, bool print_steps);					//check all the positions in a sudoku for a number that is the sole possibility in the box
	void checkRows(SUDOKU &sudoku, bool print_steps);					//check the rows of a given sudoku for a number only possible in one place in the row
	void checkColumns(SUDOKU &sudoku, bool print_steps);					//check the columns of a given sudoku for a number only possible in one place in the column
	void checkBox(SUDOKU &sudoku, bool print_steps);					//check each 3x3 box of a sudoku for a number who has only one spot
	void nakedPair(SUDOKU &sudoku, bool print_steps);					//look for a naked pair
	void nakedTriple(SUDOKU &sudoku, bool print_steps);					//search for naked triples
	void hiddenPair(SUDOKU &sudoku, bool print_steps);					//search for hidden pairs TODO:Test if it truly works 
	void hiddenTriple(SUDOKU &sudoku, bool print_steps);					//TODO:Search for hidden triples
	void pointingBoxRows(SUDOKU &sudoku, bool print_steps);					//check each 3x3 box of sudoku's rows for a number whose sole possiblities lie in that row
	void pointingBoxColumns(SUDOKU &sudoku, bool print_steps);				//check each 3x3 box of sudoku's columns for a number whose sole possiblities lie in that column
	void boxLineReduceRow(SUDOKU &sudoku, bool print_steps);				//check if numbers manifest themselves in 2 boxes such that in box 3 numbers can be eliminated
	void boxLineReduceColumn(SUDOKU &sudoku, bool print_steps);				//check if numbers manifest themselves in 2 boxes such that in box 3 numbers can be eliminated
	void xWing(SUDOKU &sudoku, bool print_steps);						//x-wing algorithm
	void yWing(SUDOKU &sudoku, bool print_steps);						//y-wing algorithm
	void trialError(SUDOKU &sudoku, bool print_steps);					//brute force the solution (Bowman's Bingo')
}
