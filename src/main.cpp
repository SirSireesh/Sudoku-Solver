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
#include "termcolor.hpp"

using namespace CSudokuSolver;

int main(int argc, char *argv[])
{
	bool print_steps = false, silent = false, logical = false;
	char *name = argv[0];
	while (--argc > 0 && (*++argv)[0] == '-')
	{
		char c;
		while((c = *++argv[0]))
			switch (c)
			{
				case 'a':
					std::cout << "ssolver (C) 2016 Kiran Dhana and Sireesh Kodali.\n";
					std::cout << "This program comes with ABSOLUTLY NO WARRANTY; for details check license.txt\n";
					std::cout << "This program is free software, and you are welcome to redistribute it under\n";
					std::cout << "certain conditions; check license.txt for more details\n";
					return 0;
				case 'h':
					std::cout << name << " : version 0.10.2 (Eliza)\n\n";
					std::cout << "Usage : " << name << " [arguments]\n";
					std::cout << "Arguments:\n";
					std::cout << " -a\t Print license info and exit\n";
					std::cout << " -h\t Print this help menu and exit\n";
					std::cout << " -l\t Sove the sudoku logically (no guesses/trial and error)\n";
					std::cout << " -s\t Only print unformatted input and answer (useful for automated solving of sudokus)\n";
					std::cout << " -t\t Print how to solve the given sudoku (step by step solution!)\n";
					std::cout << " -v\t Print version info and exit\n";
					return 0;
				case 't' :
					if (!silent)
						print_steps = true;
					else
					{
						std::cerr << "Conflicting options! -s and -t can not be used together!\nExiting!\n";
						return 1;
					}
					break;
				case 'v' :
					std::cout << name << " : version 0.10.2 (Eliza)\n";
					return 0;
				case 's' :
					if (!print_steps)
						silent = true;
					else
					{
						std::cerr << "Conflicting options! -s and -t can not be used together\nExiting!\n"; 
						return 1;
					}
					break;
				case 'l' :
					logical = true;
					break;
				default :
					std::cerr << name << " invalid option : " << c << '\n';
					std::cerr << "Usage : " << name << " [arguments]\n";
					std::cerr << "Try \'" << name << " -h\' for more information\n";
					return 1;
			}
	}

	SUDOKU sudoku;

	if (!getSudoku(sudoku))
	{
		std::cerr << name << " : The input string was too short, too long or contained invalid characters\n";
		std::cout << "Check your input and try agin!\nExiting ...\n";
		return 2;
	}

	if (!initialiseSudoku(sudoku) || count(sudoku.sudoku_q) < 17) 
	{ 
		std::cerr << name <<  "The input sudoku is invalid! It contains too few clues or an invalid question.\n";
		printSudoku(sudoku.sudoku_q);
		return 2;
	}

	if (!silent)
	{
		std::cout << "The given sudoku is :\n";
		printfSudoku(sudoku);
		std::cout << "Given : " << termcolor::red << count(sudoku.sudoku_q) << termcolor::reset << '\n';
	}

	auto sTime = std::chrono::high_resolution_clock::now();

	while (count(sudoku.sudoku_a) < 81 && sudoku.changed) 
	{
		sudoku.changed = false;
		nakedSingle(sudoku, print_steps);
		if (!sudoku.changed)
		{
			checkColumns(sudoku, print_steps);
			checkRows(sudoku, print_steps);
			checkBox(sudoku, print_steps);
		}
		if (!sudoku.changed) 
			nakedPair(sudoku, print_steps);
		if (!sudoku.changed)
			nakedTriple(sudoku, print_steps);
		if (!sudoku.changed)
			pointingBoxColumns(sudoku, print_steps);
		if (!sudoku.changed)
			pointingBoxRows(sudoku, print_steps);
		if (!sudoku.changed)
			boxLineReduceRow(sudoku, print_steps);
		if (!sudoku.changed)
			boxLineReduceColumn(sudoku, print_steps);
		if (!sudoku.changed)
			xWing(sudoku, print_steps);
		if (!sudoku.changed)
			yWing(sudoku, print_steps);
		if (!sudoku.changed && !logical)
			trialError(sudoku, print_steps);
	}

	auto eTime = std::chrono::high_resolution_clock::now();	

	if (checkError(sudoku)) 
	{
		std::cerr << name << "Something went wrong while solving the sudoku! Are you sure the give sudoku is valid?!\n";
		printSudoku(sudoku.sudoku_a);
		return 3;
	}

	if (!silent)
	{
		printfSudoku(sudoku);
		std::cout << "Answered : " << termcolor::green << count(sudoku.sudoku_a) << termcolor::reset << '\n';
		std::cout << "Time taken = " << std::chrono::duration_cast<std::chrono::nanoseconds>(eTime - sTime).count() * 1E-6 << " milliseconds\n";
	}
	else 
		printSudoku(sudoku.sudoku_a);

	return 0;
}
