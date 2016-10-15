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

int main(int argc, char *argv[])
{
	bool print_steps = false;
	while (--argc > 0 && (*++argv)[0] == '-')
	{
		char c;
		while((c = *++argv[0]))
			switch (c)
			{
				case 'l':
					std::cout << "Sudoku-solver (C) 2016 Kiran Dhana and Sireesh Kodali.\n";
					std::cout << "This program comes with ABSOLUTLY NO WARRANTY; for details check license.txt\n";
					std::cout << "This program is free software, and you are welcome to redistribute it under\n";
					std::cout << "certain conditions; check license.txt for more details\n\n";
					return 0;
				case 'h':
					std::cout << "Sudoku-solver : version 0.7 (Batman)\n\n";
					std::cout << "usage : Sudoku-solver [arguments] < [sudoku]\n";
					std::cout << "Arguments: ";
					std::cout << " -l\t Print license info and exit\n";
					std::cout << " -h\t Print this help menu and exit\n";
					std::cout << " -i\t Read input from stdin (default)\n";
					std::cout << " -t\t Print how to solve the given sudoku (step by step solution!)\n";
					std::cout << " -v\t Print version info and exit\n";
					return 0;
				case 'i' :
					break;
				case 't' :
					print_steps = true;
					break;
				case 'v' :
					std::cout << "Sudoku-solver : version 0.8.1 (Calvin)\n";
					std::cout << "Built with command: $ clang++ -I. Sudoku-solver.cpp main.cpp -o ./bin/Sudoku-solver -std=c++11 -O3 -march=native\n";
					return 0;
				default :
					std::cout << RED << "Invalid Option : " << c << RESET << '\n';
					std::cout << "usage : Sudoku-solver [arguments] < [sudoku]\n";
					std::cout << "Arguments: ";
					std::cout << " -l\t Print license info and exit\n";
					std::cout << " -h\t Print this help menu and exit\n";
					std::cout << " -i\t Read input from stdin (default)\n";
					std::cout << " -t\t Print how to solve the given sudoku (step by step solution!)\n";
					std::cout << " -v\t Print version info and exit\n";
					return -1;
			}
	}

	SUDOKU_ANS_BOARD sudoku_ans;
	int sudoku_q[9][9];

	if (!inputSudoku(sudoku_q))
	{
		std::cout << "Exiting ...\n";
		return -2;
	}

	int sudoku_a[9][9];
	for (int i = 0; i < 9; ++i)
		for (int j = 0; j < 9; ++j)
			sudoku_a[i][j] = sudoku_q[i][j];

	if (!initialiseSudoku(sudoku_q, sudoku_ans) || count(sudoku_q) < 17) 
	{ 
		std::cerr<< "The input sudoku is invalid! It contains too few numbers or an impossible question.\n";
		printSudoku(sudoku_q, sudoku_a);
		std::cout << RED << "The sudoku contains " << count(sudoku_q) << " clues.\n" << RESET;
		return -1;
	}

	std::cout << "The given sudoku is :\n";
	printSudoku(sudoku_q, sudoku_q);

	std::cout << "Given : " << RED << count(sudoku_q) << RESET << '\n';

	auto sTime = std::chrono::high_resolution_clock::now();

	while (count(sudoku_a) < 81 && sudoku_ans.changed) 
	{
		sudoku_ans.changed = false;
		checkColumns(sudoku_ans, sudoku_a, print_steps);
		checkRows(sudoku_ans, sudoku_a, print_steps);
		nakedSingle(sudoku_ans, sudoku_a, print_steps);
		checkBox(sudoku_ans, sudoku_a, print_steps);
		if (!sudoku_ans.changed) 
			boxLineReduceRow(sudoku_ans, print_steps);
		if (!sudoku_ans.changed)
			boxLineReduceColumn(sudoku_ans, print_steps);
		if (!sudoku_ans.changed)
			pointingBoxColumns(sudoku_ans, print_steps);
		if (!sudoku_ans.changed)
			pointingBoxRows(sudoku_ans, print_steps);
		if (!sudoku_ans.changed)
			nakedPair(sudoku_ans, print_steps);
		if (!sudoku_ans.changed)
			nakedTriple(sudoku_ans, print_steps);
		if (!sudoku_ans.changed)
			xWing(sudoku_ans, print_steps);
	}

	auto eTime = std::chrono::high_resolution_clock::now();	

	if (checkError(sudoku_ans, sudoku_a)) 
	{
		std::cerr << "Something went wrong!\n";
		printSudoku(sudoku_q, sudoku_a);
		return -3;
	}

	printSudoku(sudoku_q, sudoku_a);

	std::cout << "Answered : " << GREEN << count(sudoku_a) << RESET << '\n';
	std::cout << "Time taken = " << std::chrono::duration_cast<std::chrono::nanoseconds>(eTime - sTime).count() * 1E-6 << " milliseconds\n";

	return 0;
}
