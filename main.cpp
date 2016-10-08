/*  This program solves a given sudoku by applying simple algorithms that mortals use to solve sudokus
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
#include <chrono>
#include <iostream>
#include "Sudoku-solver.h"

using namespace CSudokuSolver;

int main()
{
	SUDOKU_ANS_BOARD sudoku_ans;
	int sudoku_q[9][9] = {
		{0,4,0,5,1,0,0,0,0},		//0 is null value or a blank box in the sudoku
		{3,0,0,0,0,0,0,0,2},
		{5,1,0,2,6,0,9,0,0},
		{0,0,0,0,9,0,7,8,1},
		{0,0,0,0,0,0,0,0,0},
		{1,7,5,0,3,0,0,0,0},
		{0,0,9,0,4,2,0,6,8},
		{2,0,0,0,0,0,0,0,3},
		{0,0,0,0,5,6,0,9,0}};

	std::cout << "Sudoku-solver (C) 2016 Kiran Dhana and Sireesh Kodali.\n";
	std::cout << "This program comes with ABSOLUTLY NO WARRANTY; for details check license.txt\n";
	std::cout << "This program is free software, and you are welcome to redistribute it under\n";
	std::cout << "certain conditions; check license.txt for more details\n\n";

	auto sTime = std::chrono::high_resolution_clock::now();
	//TODO: Create gui instead for input
	//TODO: Read www.sudokuwiki.org

	//initialise the answer board
	if (!initialiseSudoku(sudoku_q, sudoku_ans) || count(sudoku_q) < 17) 
	{ 
		//something went wrong - the input sudoku was probably wrong
		std::cerr<< "The input sudoku is invalid! It contains too few numbers or an impossible question.\n";
		printSudoku(sudoku_q);
		std::cout << "The sudoku contains " << count(sudoku_q) << " clues.\n";
		return -1;
	}

	std::cout << "The given sudoku is :\n";
	printSudoku(sudoku_q);

	std::cout << "Given : " << count(sudoku_q) << '\n';

	while (count(sudoku_q) < 81 && sudoku_ans.changed) 
	{
		sudoku_ans.changed = false;
		checkColumns(sudoku_ans, sudoku_q);
		checkRows(sudoku_ans, sudoku_q);
		nakedSingle(sudoku_ans, sudoku_q);
		checkBox(sudoku_ans, sudoku_q);
		if (!sudoku_ans.changed)
			nakedPair(sudoku_ans);
		if (!sudoku_ans.changed)
			pointingBoxColumns(sudoku_ans);
		if (!sudoku_ans.changed)
			pointingBoxRows(sudoku_ans);
		if (!sudoku_ans.changed) 
			boxLineReduceRow(sudoku_ans);
		if (!sudoku_ans.changed)
			boxLineReduceColumn(sudoku_ans);
		if (!sudoku_ans.changed)
			hiddenPair(sudoku_ans);
	}
	
	if (checkError(sudoku_ans, sudoku_q)) 
	{
		std::cout << "Something went wrong!\n";
		printSudoku(sudoku_q);
		return -1;
	}
	auto eTime = std::chrono::high_resolution_clock::now();	
	printSudoku(sudoku_q);

	std::cout << "Answered : " << count(sudoku_q) << '\n';
	std::cout << "Time taken = " << std::chrono::duration_cast<std::chrono::nanoseconds>(eTime - sTime).count() * 1E-6 << " milliseconds\n";

	return 0;
}
