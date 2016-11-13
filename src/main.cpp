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

void getOpt(int argc, char *argv[], bool &print_steps, bool &silent, bool &logical, bool &brute_force);
void printVersion(const char name[]);
void printLicense();
void printHelp(const char name[]);

int main(int argc, char *argv[])
{
	bool print_steps = false, silent = false, logical = false, brute_force = false;		//options

	getOpt(argc, argv, print_steps, silent, logical, brute_force);

	SUDOKU sudoku;		//our sudoku board

	if (!silent)
		std::cout << "Enter the sudoku : ";
	if (!getSudoku(sudoku))
	{
		//input string error
		std::cerr << argv[0] << " : The input string was too short, too long or contained invalid characters\n";
		std::cerr << "Check your input and try agin!\nExiting ...\n";
		return 2;
	}

	if (!initialiseSudoku(sudoku) || sudoku.num_solved < 17) 
	{ 
		//input string has invalid question or the sudoku has too few clues 
		std::cerr << argv[0] <<  " : The input sudoku is invalid! It contains too few clues or an invalid question.\n";
		printSudoku(sudoku.sudoku_q);
		return 2;
	}

	if (!silent)
	{
		//print extra only if silent is not set
		std::cout << "The given sudoku is :\n";
		printfSudoku(sudoku);
		std::cout << "Given : " << termcolor::red << sudoku.num_solved << termcolor::reset << '\n';
	}

	auto sTime = std::chrono::high_resolution_clock::now();		//get current time for printing time take
	if (!brute_force)
	{
		solveSudoku(sudoku, print_steps, logical);
		if (sudoku.num_solved < 81 && !logical)
		{
			std::cerr << argv[0] << " : The sudoku could not be solved logically. Since the logical flag was not set, the program has solved it using brute force\n";
			bruteForce(sudoku);
		}
	}
	else
		bruteForce(sudoku);
	auto eTime = std::chrono::high_resolution_clock::now();		//the time after the sudoku solving was completed

	if (checkError(sudoku)) 
	{
		//Since the solver can't go wrong, the input sudoku must be invalid if there is an error
		std::cerr << argv[0] << " : Something went wrong while solving the sudoku! Are you sure the give sudoku is valid?!\n";
		printSudoku(sudoku.sudoku_a);
		return 3;
	}

	if (!silent)
	{
		//print the fancy stuff if silent is not set
		printfSudoku(sudoku);
		std::cout << "Answered : " << termcolor::green << sudoku.num_solved << termcolor::reset << '\n';
		std::cout << "Rated : ";
		for (int i = 1; i <= sudoku.rating; ++i)
			std::cout << '*';
		std::cout << '\n';
		std::cout << "Time taken = " << std::chrono::duration_cast<std::chrono::nanoseconds>(eTime - sTime).count() * 1E-6 << " milliseconds\n"; //print time in milliseconds
	}
	else 
		printSudoku(sudoku.sudoku_a);

	return 0;
}

void getOpt(int argc, char *argv[], bool &print_steps, bool &silent, bool &logical, bool &brute_force)
{
	//get all the options and list behave accordingly
	for (int i = 1; i < argc; ++i)
	{
		if (strcmp(argv[i], "--about") == 0)
		{
			printLicense();
			exit(0);
		}
		else if (strcmp(argv[i], "--brute-force") == 0)
		{
			brute_force = true;
			if (logical)
				std::cerr << argv[0] << "Conflicting options! --brute-force and --logical can not be used together, the sudoku will be solved with brute force only\n";
		}
		else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-help") == 0)
		{
			printHelp(argv[0]);
			exit(0);
		}
		else if (strcmp(argv[i], "--logical") == 0)
		{
			if (brute_force)
				std::cerr << argv[0] << "Conflicting options! --brute-force and --logical can not be used together, the sudoku will be solved with brute force only\n";
			else
				logical = true;
		}
		else if (strcmp(argv[i], "--trace") == 0)
		{
			if (!silent)
				print_steps = true;
			else
			{
				std::cerr << argv[0] << "Conflicting options! --silent and --trace can not be used together!\nExiting!\n";
				exit(1);
			}
		}
		else if (strcmp(argv[i], "--silent") == 0)
		{
			if (!print_steps)
				silent = true;
			else 
			{
				std::cerr << argv[0] << "Conflicting options! --silent and --trace can not be used together!\nExiting!\n";
				exit(1);
			}
		}
		else if (strcmp(argv[i], "--version") == 0)
		{
			printLicense();
			exit(0);
		}
		else if (argv[i][0] == '-' && argv[i][1] != '-')
		{
			//if it was none of the above, it must have been one of the one letter options
			for (int j = 1; argv[i][j] != '\0'; ++j)
				switch (argv[i][j])
				{
					case 'a':
						printLicense();
						exit(0);
					case 'b':
						brute_force = true;
						if (logical)
							std::cerr << argv[0] << "Conflicting options! -b and -l can not be used together, the sudoku will be solved with brute force only\n";
						break;
					case 'h':
						printHelp(argv[0]);
						exit(0);
					case 't' :
						if (!silent)
							print_steps = true;
						else
						{
							std::cerr << argv[0] << "Conflicting options! -s and -t can not be used together!\nExiting!\n";
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
							std::cerr << argv[0] << "Conflicting options! -s and -t can not be used together\nExiting!\n"; 
							exit(1);
						}
						break;
					case 'l' :
						if (brute_force)
							std::cerr << argv[0] << "Conflicting options! -b and-l can not be used together, the sudoku will be solved with brute force only\n";
						else
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
			//since we don't suppport file handling for now, any other options must be mistakes
			std::cerr << argv[0] << " invalid option : " << argv[i] << '\n';
			std::cerr << "Usage : " << argv[0] << " [options]\n";
			std::cerr << "Try \'" << argv[0] << " -h\' for more information\n";
			exit(1);
		}
	}
}

void printHelp(const char name[])
{
	//print help menu
	printVersion(name);
	std::cout << "Usage : " << name << " [options]\n";
	std::cout << "Options:\n";
	std::cout << " -a  --about	\t Print license info and exit\n";
	std::cout << " -b  --brute-force\t Solve the sudoku using Brute Force™\n";
	std::cout << " -h  --help	\t Print this help menu and exit\n";
	std::cout << " -l  --logical	\t Sove the sudoku logically (no guesses/trial and error)\n";
	std::cout << " -s  --silent 	\t Only print unformatted input and answer (useful for automated solving of sudokus)\n";
	std::cout << " -t  --trace  	\t Print how to solve the given sudoku (step by step solution!)\n";
	std::cout << " -v  --version	\t Print version info and exit\n";
}

void printVersion(const char name[])
{
	//print version
	std::cout << name << " : version 0.10.2 (Eliza)\n\n";
}

void printLicense()
{
	//print license, as per GPL requirements
	std::cout << "ssolver (C) 2016 Kiran Dhana and Sireesh Kodali.\n";
	std::cout << "This program comes with ABSOLUTLY NO WARRANTY; for details check license.txt\n";
	std::cout << "This program is free software, and you are welcome to redistribute it under\n";
	std::cout << "certain conditions; check license.txt for more details\n";
}
