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
#include <cstring>
#include <cstdlib>

#include "Sudoku-solver.h"
#include "termcolor.hpp"

using namespace CSudokuSolver;

void getOpt(int argc, char *argv[], bool &print_steps, bool &silent, bool &logical);
void printVersion(const char name[]);
void printLicense();
void printHelp(const char name[]);

int main(int argc, char *argv[])
{
	bool print_steps = false, silent = false, logical = false;
	const char *prog_name = argv[0];

	getOpt(argc, argv, print_steps, silent, logical);

	SUDOKU sudoku;

	if (!getSudoku(sudoku))
	{
		std::cerr << prog_name << " : The input string was too short, too long or contained invalid characters\n";
		std::cout << "Check your input and try agin!\nExiting ...\n";
		return 2;
	}

	if (!initialiseSudoku(sudoku) || count(sudoku.sudoku_q) < 17) 
	{ 
		std::cerr << prog_name <<  " : The input sudoku is invalid! It contains too few clues or an invalid question.\n";
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
		std::cerr << prog_name << " : Something went wrong while solving the sudoku! Are you sure the give sudoku is valid?!\n";
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

void getOpt(int argc, char *argv[], bool &print_steps, bool &silent, bool &logical)
{
	for (int i = 1; i < argc; ++i)
	{
		if (strcmp(argv[i], "--about") == 0)
		{
			printLicense();
			exit(0);
		}
		else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-help") == 0)
		{
			printHelp(argv[0]);
			exit(0);
		}
		else if (strcmp(argv[i], "--trace") == 0)
		{
			if (!silent)
				print_steps = true;
			else
			{
				std::cerr << "Conflicting options! --silent and --trace can not be used together!\nExiting!\n";
				exit(1);
			}
		}
		else if (strcmp(argv[i], "--silent") == 0)
		{
			if (!print_steps)
				silent = true;
			else 
			{
				std::cerr << "Conflicting options! --silent and --trace can not be used together!\nExiting!\n";
				exit(1);
			}
		}
		else if (strcmp(argv[i], "--version") == 0)
		{
			printLicense();
			exit(0);
		}
		else if (argv[i][0] == '-')
		{
			for (int j = 1; argv[i][j] != '\0'; ++j)
				switch (argv[i][j])
				{
					case 'a':
						printLicense();
						exit(0);
					case 'h':
						printHelp(argv[0]);
						exit(0);
					case 't' :
						if (!silent)
							print_steps = true;
						else
						{
							std::cerr << "Conflicting options! -s and -t can not be used together!\nExiting!\n";
							exit(1);
						}
						break;
					case 'v' :
						printVersion(argv[0]);
						exit(0);
					case 's' :
						if (!print_steps)
							silent = true;
						else
						{
							std::cerr << "Conflicting options! -s and -t can not be used together\nExiting!\n"; 
							exit(1);
						}
						break;
					case 'l' :
						logical = true;
						break;
					default :
						std::cerr << argv[0] << " invalid option : " << argv[i][j] << '\n';
						std::cerr << "Usage : " << argv[0] << " [options]\n";
						std::cerr << "Try \'" << argv[0] << " -h\' for more information\n";
						exit(1);
				}
		}
		else
		{
			std::cerr << argv[0] << " invalid option : " << argv[i] << '\n';
			std::cerr << "Usage : " << argv[0] << " [options]\n";
			std::cerr << "Try \'" << argv[0] << " -h\' for more information\n";
			exit(1);
		}
	}
}

void printHelp(const char name[])
{
	printVersion(name);
	std::cout << "Usage : " << name << " [options]\n";
	std::cout << "Options:\n";
	std::cout << " -a\t Print license info and exit\n";
	std::cout << " -h\t Print this help menu and exit\n";
	std::cout << " -l\t Sove the sudoku logically (no guesses/trial and error)\n";
	std::cout << " -s\t Only print unformatted input and answer (useful for automated solving of sudokus)\n";
	std::cout << " -t\t Print how to solve the given sudoku (step by step solution!)\n";
	std::cout << " -v\t Print version info and exit\n";
}

void printVersion(const char name[])
{
	std::cout << name << " : version 0.10.2 (Eliza)\n\n";
}

void printLicense()
{
	std::cout << "ssolver (C) 2016 Kiran Dhana and Sireesh Kodali.\n";
	std::cout << "This program comes with ABSOLUTLY NO WARRANTY; for details check license.txt\n";
	std::cout << "This program is free software, and you are welcome to redistribute it under\n";
	std::cout << "certain conditions; check license.txt for more details\n";
}
