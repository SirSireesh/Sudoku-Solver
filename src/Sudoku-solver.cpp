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

//#define DEBUG

#include <iostream>
#include <list>
#include <algorithm>
#include <iterator>
#include <string>

#include "Sudoku-solver.h"
#include "termcolor.hpp"

bool CSudokuSolver::getSudoku(SUDOKU &sudoku)
{
	//get the input as string from stdin,
	//read store in the 9x9 sudokus in struct SUDOKU
	//return false if something went wrong
	std::string input;
	std::getline(std::cin, input);

	if (input.length() != 81)
		return false;

	for (int i = 0; i < 9; ++i)
		for (int j = 0; j < 9; ++j)
		{
			switch (input[(i * 9) + j])
			{
				case '1': case '2' : case '3' : case '4' : case '5' : case '6' : case '7' : case '8' : case '9' :
					sudoku.sudoku_q[i][j] = sudoku.sudoku_a[i][j] = static_cast<int> (input[(i * 9) + j] - '0');
					break;
				case '0' : case '.' : case ' ' : 
					sudoku.sudoku_q[i][j] = sudoku.sudoku_a[i][j] = 0;
					break;
				default :
					return false;
			}
		}
	return true;
}

bool CSudokuSolver::initialiseSudoku(SUDOKU &sudoku)
{
	//from the given 9x9 sudokus in struct SUDOKU, initialize the possibilities 
	//return false if the question is not valid
	for (int i = 0; i < 9; ++i)
		for (int j = 0; j < 9; ++j) 
		{
			if (sudoku.sudoku_q[i][j] && sudoku.sudoku_ans.cell[i][j].num[sudoku.sudoku_q[i][j] - 1] == false) 
				return false;
			if (sudoku.sudoku_q[i][j])
				finalize(sudoku, sudoku.sudoku_q[i][j] - 1, i, j, true);
		}
	return true;
}

void CSudokuSolver::printSudoku(int sudoku_q[9][9])
{
	//print the sudoku, unformatted, as a single line string
	for (int i = 0; i < 9; ++i)
		for (int j = 0; j < 9; ++j)
			std::cout << sudoku_q[i][j];
	std::cout << '\n';
}

void CSudokuSolver::printfSudoku(SUDOKU sudoku)
{
	//print the sudoku with all the fancy extras
	//prints black chars for given q and red for nums solved by the program
	std::cout << "\t\t   " << termcolor::magenta << "1 2 3 4 5 6 7 8 9\n";
	for (int i = 0; i < 9; ++i) 
	{
		std::cout << "\t\t" << termcolor::magenta << static_cast<char> (i + 65) << termcolor::blue << " "; 
		for (int j = 0; j < 9; ++j)
		{
			if (j - j % 3 == i - i % 3)
				std::cout << termcolor::on_yellow;
			else if (6 - (i - i % 3) == j - j % 3)
				std::cout << termcolor::on_yellow;
			else
				std::cout << termcolor::on_white;
			if (!sudoku.sudoku_q[i][j] && sudoku.sudoku_a[i][j])
				std::cout << termcolor::red << sudoku.sudoku_a[i][j] << termcolor::blue << " ";
			else if (!sudoku.sudoku_q[i][j] && !sudoku.sudoku_a[i][j])
				std::cout << termcolor::blue <<"  " << termcolor::reset;
			else if (sudoku.sudoku_q[i][j])
				std::cout << termcolor::grey << sudoku.sudoku_q[i][j] << termcolor::blue << " ";
		}
		std::cout << termcolor::reset << '\n';
	}
}

int CSudokuSolver::numCommon(SUDOKU sudoku, POINT pos1, POINT pos2)
{
	//return the number of common numbers between 2 positions
	int commmon = 0;
	for (int n = 0; n < 9; ++n)
		if (sudoku.sudoku_ans.cell[pos1.x][pos1.y].num[n] && sudoku.sudoku_ans.cell[pos2.x][pos2.y].num[n])
			++commmon;
	return commmon;
}

int CSudokuSolver::getCommon(SUDOKU sudoku, POINT pos1, POINT pos2)
{
	//return the first common number between 2 positions
	//return -1 if none were found
	for (int n = 0; n < 9; ++n)
		if (sudoku.sudoku_ans.cell[pos1.x][pos1.y].num[n] && sudoku.sudoku_ans.cell[pos2.x][pos2.y].num[n])
			return n;
	return -1;
}

int CSudokuSolver::numPossible(SUDOKU sudoku, int x, int y)
{
	int possible = 0;
	if (sudoku.sudoku_ans.cell[x][y].done == true)
		return 0;
	else
		for (int n = 0; n < 9; ++n)
			if (sudoku.sudoku_ans.cell[x][y].num[n])
				++possible;
	return possible;
}

CSudokuSolver::POINT CSudokuSolver::leastNumsPos(SUDOKU sudoku)
{
	int min_possible = 100;
	POINT min_point = {-1, -1};
	for (int i = 0; i < 9; ++i)
		for (int j = 0; j < 9; ++j)
		{
			if (sudoku.sudoku_ans.cell[i][j].done == false)
			{
				int cposs = numPossible(sudoku, i, j);
				if (cposs < min_possible)
				{
					min_point.x = i;
					min_point.y = j;
					min_possible = cposs;
				}
			}
		}
	return min_point;
}

void CSudokuSolver::disablePos(SUDOKU &sudoku, int n, int x, int y)
{
	//disable a number at a poition on the possibilities board
	//POINT struct is not used as the performance loss is significant and more unnecessay variables 'temp' variable are created
	//don't change anything if the box is already solved or the num is already marked as false in that position
	if (sudoku.sudoku_ans.cell[x][y].num[n] == true && sudoku.sudoku_ans.cell[x][y].done == false) 
	{
		sudoku.changed = true;
		sudoku.sudoku_ans.cell[x][y].num[n] = false;
	}	
}

void CSudokuSolver::disableColumn(SUDOKU &sudoku, int n, int column)
{
	//disable a given number for a column
	//calls disablePos to do the dirty work
	for (int i = 0; i < 9; ++i) 
		if (sudoku.sudoku_ans.cell[i][column].done == false)
			disablePos(sudoku, n, i, column);
}

void CSudokuSolver::disableRow(SUDOKU &sudoku, int n, int row)
{
	//disable a given number for a row
	//calls disablePos to do the dirty work
	for (int j = 0; j < 9; ++j) 
		if (sudoku.sudoku_ans.cell[row][j].done == false)
			disablePos(sudoku, n, row, j);
}

void CSudokuSolver::disableBox(SUDOKU &sudoku, int n, int x, int y)
{
	//disable a given number for a box
	//calls disablePos to do the dirty work
	for (int i = x - (x%3); i < (x - (x%3) + 3); ++i)
		for (int j = y - (y%3); j < (y  - (y%3) + 3); ++j) 
			if (sudoku.sudoku_ans.cell[i][j].done == false)
				disablePos(sudoku, n, i, j);
}

void CSudokuSolver::finalize(SUDOKU &sudoku, int n, int x, int y, bool init = false)
{
	//finalize a given number at a position
#ifdef DEBUG
	if (sudoku.sudoku_ans.cell[x][y].num[n] == false) 
	{
		//this should not happen, but no harm done preventing errors
		//diabling this might give a slight boost on performance
		std::cout << termcolor::red << "error : The number " << n + 1 << " was set to false at " << x << ' ' << y << "!\n";
		return;
	}
	if (sudoku.sudoku_ans.cell[x][y].done) 
	{
		//again, this should not happen, but no harm done preventing
		//disabling this might give a slight boost in performance
		std::cout << termcolor::red << "error : The position " << x << ' ' << y << " was set to done!" << " Number " << sudoku.sudoku_q[x][y] << " is already there!\n";
		return;
	}
#endif
	sudoku.sudoku_ans.cell[x][y].done = true;
	sudoku.changed = true;
	if (init)
		sudoku.sudoku_q[x][y] = sudoku.sudoku_a[x][y] = n + 1;
	else
		sudoku.sudoku_a[x][y] = n + 1;

	//disable all numbers in the cell to prevent silly bugs
	for (int i = 0; i < 9; ++i) 
		sudoku.sudoku_ans.cell[x][y].num[i] = false;
	//disable the number for all cells that it 'sees'
	disableRow(sudoku, n, x);
	disableColumn(sudoku, n, y);
	disableBox(sudoku, n, x, y);
	++sudoku.num_solved;
}

void CSudokuSolver::checkColumns(SUDOKU &sudoku, bool print_steps = false)
{
	//check the columns of the possiblities board
	//if a number fits in only one place for a column, it is its final place
	bool only_pos[9];

	for (int i = 0; i < 9; ++i) 
	{
		for (auto& elem : only_pos)
			elem = true;
		for (int j = 0; j < 9; ++j) 
			if (sudoku.sudoku_ans.cell[j][i].done == false)
				for (int n = 0; n < 9; ++n)
					if (sudoku.sudoku_ans.cell[j][i].num[n] && only_pos[n]) 
					{
						for (int k = j + 1; k < 9; ++k)
							if (sudoku.sudoku_ans.cell[k][i].done == false && sudoku.sudoku_ans.cell[k][i].num[n]) 
							{
								only_pos[n] = false;
								break;
							}
						if (only_pos[n])
						{
							finalize(sudoku, n, j, i);
							if (print_steps)
								std::cout << termcolor::green << "Single : " << termcolor::magenta << static_cast<char> (j + 65) << i + 1 
									<< termcolor::reset << " set to " << termcolor::green << n + 1 << termcolor::reset << " : unique in column\n";
						}
					}
	}
}

void CSudokuSolver::checkRows(SUDOKU &sudoku, bool print_steps = false)
{
	//check the rows of the possibilities board
	//if a number fits in only one place for a row, then that is its final place
	bool only_pos[9];

	for (int i = 0; i < 9; ++i)
	{
		for (auto& elem : only_pos)		
			elem = true;
		for (int j = 0; j < 9; ++j) 
			if (sudoku.sudoku_ans.cell[i][j].done == false) 	
				for (int n  = 0; n < 9; ++n)
					if (sudoku.sudoku_ans.cell[i][j].num[n] && only_pos[n]) 
					{
						for (int k = j + 1; k < 9; ++k)
							if (sudoku.sudoku_ans.cell[i][k].done == false && sudoku.sudoku_ans.cell[i][k].num[n]) 
							{
								only_pos[n] = false;
								break;
							}
						if (only_pos[n]) 
						{
							finalize(sudoku, n, i, j);
							if (print_steps)
								std::cout << termcolor::green << "Single : " << termcolor::magenta << static_cast<char> (i + 65) << j + 1 
									<< termcolor::reset <<" set to " << termcolor::green << n + 1 << termcolor::reset << " : unique in row\n";
						}
					}
	}
}

void CSudokuSolver::checkBox(SUDOKU &sudoku, bool print_steps = false)
{
	//check within a 3x3 box if there is a number that fits only in one place
	//if there exists such a number, that is its final place
	bool only_pos[9];

	for (int i = 0; i < 9; i += 3) 
	{
		for (int j = 0; j < 9; j += 3) 
		{
			for (auto& elem : only_pos)
				elem = true;
			for (int k = i; k < i + 3 && k < 9; ++k) 
				for (int l = j; l < j + 3 && j < 9; ++l) 
					if (sudoku.sudoku_ans.cell[k][l].done == false)
						for (int n = 0; n < 9; ++n)
							if (sudoku.sudoku_ans.cell[k][l].num[n] && only_pos[n]) 
							{
								for (int m = i; m < i + 3; ++m) 
									for (int o = j; o < j + 3; ++o) 
										if (sudoku.sudoku_ans.cell[m][o].done == false) 
										{
											if ((m != k || o != l) && (sudoku.sudoku_ans.cell[m][o].num[n]))
											{
												only_pos[n] = false;
												break;
											}
										}
								if (only_pos[n])
								{
									finalize(sudoku, n, k, l);
									if (print_steps)
										std::cout << termcolor::green << "Single : " << termcolor::magenta << static_cast<char> (k + 65) << l + 1 
											<< termcolor::reset << " set to " << termcolor::green << n + 1 << termcolor::reset << " : unique in 3x3 box\n";
								}
							}
		}
	}
}

void CSudokuSolver::nakedSingle(SUDOKU &sudoku, bool print_steps = false)
{
	//in a cell, if there is only one possible number, that is its final place
	int poss = 0;
	int num = 0;
	for (int i = 0; i < 9; ++i)
		for (int j = 0; j < 9; ++j)
		{
			poss = 0;
			if (sudoku.sudoku_ans.cell[i][j].done == false)
				for (int n = 0; n < 9; ++n)
					if (sudoku.sudoku_ans.cell[i][j].num[n] == true) 
					{
						++poss;
						num = n;
					}
			if (poss == 1)
			{
				finalize(sudoku, num, i, j);
				if (print_steps)
					std::cout << termcolor::green << "Single : " << termcolor::magenta << static_cast<char> (i + 65) << j + 1 
						<< termcolor::reset << " set to " << termcolor::green << num + 1 <<  termcolor::reset << " : unique in box\n";
			}
		}
}

void CSudokuSolver::nakedPair(SUDOKU &sudoku, bool print_steps = false)
{
	//if there are two numbers such that they ar the only 2 possible numbers
	//in two separate boxes within the same unit
	//the same numbes can be disabled everywhere else in the same unit

	//exit on finding a pair, as our 'counter' becomes outdated
	int counter[9][9];
	for (int i = 0; i < 9; ++i)
		for (int j = 0; j < 9; ++j)
		{
			counter[j][i] = 0;
			if (sudoku.sudoku_ans.cell[j][i].done == false)
				for (int n = 0; n < 9; ++n)
					if (sudoku.sudoku_ans.cell[j][i].num[n])
						++counter[j][i];
		}

	std::list<int> nums;
	std::list<int> rows;

	for (int i = 0; i < 9; ++i)
	{
		nums.clear();
		rows.clear();
		for (int j = 0; j < 9; ++j)
		{
			if (counter[j][i] == 2)
				rows.push_back(j);
		}
		if (rows.size() >= 2)
		{
			for (auto row1 = rows.begin(); row1 != rows.end(); ++row1)
			{
				for (auto row2 = std::next(row1, 1); row2 != rows.end(); ++row2)
				{
					nums.clear();
					for (int n = 0; n < 9; ++n)
					{
						if ((nums.size() == 0 || std::find(nums.begin(), nums.end(), n) == nums.end()) && 
								(sudoku.sudoku_ans.cell[*row1][i].num[n]
								 || sudoku.sudoku_ans.cell[*row2][i].num[n]))
							nums.push_back(n);
					}
					if (nums.size() == 2)
					{
						for (int j = 0; j < 9; ++j)
						{
							if (j != *row1 && j != *row2 && sudoku.sudoku_ans.cell[j][i].done == false)
							{
								for (auto num : nums)
									disablePos(sudoku, num, j, i);
							}
						}
						if (sudoku.changed)
						{
							if (print_steps)
							{
								std::cout << termcolor::green << "Naked Pair (Column) : " << termcolor::magenta;
								std::cout << static_cast<char> (*row1 + 65) << i + 1 << ' ' << static_cast<char> (*row2 + 65) << i + 1
									<< termcolor::reset << " removes " << termcolor::green;
								for (auto num : nums)
									std::cout << num + 1 << ' ';
								std::cout << termcolor::reset << '\n';
								return;
							}
						}
					}
				}
			}
		}
	}	

	std::list<int> columns;
	for (int i = 0; i < 9; ++i)
	{
		nums.clear();
		columns.clear();
		for (int j = 0; j < 9; ++j)
		{
			if (counter[i][j] == 2)
				columns.push_back(j);
		}
		if (columns.size() >= 2)
		{
			for (auto column1 = columns.begin(); column1 != columns.end(); ++column1)
			{
				for (auto column2 = std::next(column1, 1); column2 != columns.end(); ++column2)
				{
					nums.clear();
					for (int n = 0; n < 9; ++n)
					{
						if ((nums.size() == 0 || std::find(nums.begin(), nums.end(), n) == nums.end()) && 
								(sudoku.sudoku_ans.cell[i][*column1].num[n]
								 || sudoku.sudoku_ans.cell[i][*column2].num[n]))
							nums.push_back(n);
					}
					if (nums.size() == 2)
					{
						for (int j = 0; j < 9; ++j)
						{
							if (std::find(columns.begin(), columns.end(), j) == columns.end() && sudoku.sudoku_ans.cell[i][j].done == false)
							{
								for (auto num : nums)
									disablePos(sudoku, num, i, j);
							}
						}
						if (sudoku.changed)
						{
							if (print_steps)
							{
								std::cout << termcolor::green << "Naked Pair (Row) : " << termcolor::magenta;
								std::cout << static_cast<char> (i + 65) << *column1 + 1 << ' ' << static_cast<char> (i + 65) << *column2 + 1
									<< termcolor::reset << " removes " << termcolor::green;
								for (auto num : nums)
									std::cout << num + 1 << ' ';
								std::cout << termcolor::reset << '\n';
							}
							return;
						}
					}
				}
			}
		}
	}

	POINT temp;
	std::list<POINT> positions;

	for (int i = 0; i <= 6; i += 3)
	{
		for (int j = 0; j <= 6; j += 3)
		{
			positions.clear();
			for (int k = i; k < i + 3; ++k)
			{
				for (int l = j; l < j + 3; ++l)
				{
					if (counter[k][l] == 2)
					{
						temp.x = k;
						temp.y = l;
						positions.push_back(temp);
					}
				}
			}
			if (positions.size() >= 2)
			{
				for (auto pos1 = positions.begin(); pos1 != positions.end(); ++pos1)
				{
					for (auto pos2  = std::next(pos1, 1); pos2 != positions.end(); ++pos2)
					{
						nums.clear();
						for (int n = 0; n < 9; ++n)
						{
							if ((nums.size() == 0 || std::find(nums.begin(), nums.end(), n) == nums.end()) && 
									(sudoku.sudoku_ans.cell[pos1->x][pos1->y].num[n]
									 || sudoku.sudoku_ans.cell[pos2->x][pos2->y].num[n]))
								nums.push_back(n);
						}
						if (nums.size() == 2)
						{
							for (int k = i; k < i + 3; ++k)
							{
								for (int l = j; l < j + 3; ++l)
								{
									if ((k != pos2->x || l != pos2->y) && (k != pos1->x || l != pos1->y))
									{
										for (auto num : nums)
											disablePos(sudoku, num, k, l);
									}
								}
							}
							if (sudoku.changed)
							{
								if (print_steps)
								{
									std::cout << termcolor::green << "Naked Pair (Box) : " << termcolor::magenta;
									std::cout << static_cast<char> (pos1->x + 65) << pos1->y + 1 << ' ' << static_cast<char> (pos2->x + 65) << pos2->y + 1
										<< termcolor::reset << " removes " << termcolor::green;
									for (auto num : nums)
										std::cout << num + 1 << ' ';
									std::cout << termcolor::reset << '\n';
								}
								return;
							}
						}
					}
				}
			}
		}
	}
}

void CSudokuSolver::nakedTriple(SUDOKU &sudoku, bool print_steps = false)
{
	//Similar to naked double, but extended to 3 numbers
	int counter[9][9];
	for (int i = 0; i < 9; ++i)
		for (int j = 0; j < 9; ++j)
		{
			counter[j][i] = 0;
			if (sudoku.sudoku_ans.cell[j][i].done == false)
				for (int n = 0; n < 9; ++n)
					if (sudoku.sudoku_ans.cell[j][i].num[n])
						++counter[j][i];
		}

	std::list<int> nums;
	std::list<int> rows;

	for (int i = 0; i < 9; ++i)
	{
		nums.clear();
		rows.clear();
		for (int j = 0; j < 9; ++j)
		{
			if (counter[j][i] == 2 || counter[j][i] == 3)
				rows.push_back(j);
		}
		if (rows.size() >= 3)
		{
			for (auto row1 = rows.begin(); row1 != rows.end(); ++row1)
			{
				for (auto row2 = std::next(row1, 1); row2 != rows.end(); ++row2)
				{
					for (auto row3 = std::next(row2, 1); row3 != rows.end(); ++row3)
					{
						nums.clear();
						for (int n = 0; n < 9; ++n)
						{
							if ((nums.size() == 0 || std::find(nums.begin(), nums.end(), n) == nums.end()) && 
									(sudoku.sudoku_ans.cell[*row1][i].num[n]
									 || sudoku.sudoku_ans.cell[*row2][i].num[n]
									 || sudoku.sudoku_ans.cell[*row3][i].num[n]))
								nums.push_back(n);
						}
						if (nums.size() == 3)
						{
							for (int j = 0; j < 9; ++j)
							{
								if (j != *row1 && j != *row2 && j != *row3 && sudoku.sudoku_ans.cell[i][j].done == false)
								{
									for (auto num : nums)
										disablePos(sudoku, num, j, i);
								}
							}
							if (sudoku.changed)
							{
								if (print_steps)
								{
									std::cout << termcolor::green << "Naked Triple (Column) : " << termcolor::magenta;
									std::cout << static_cast<char> (*row1 + 65) << i + 1 << ' ' << static_cast<char> (*row2 + 65) << i + 1 << ' '
										<< static_cast<char> (*row3 + 65) << i + 1 << termcolor::reset << " removes " << termcolor::green;
									for (auto num : nums)
										std::cout << num + 1 << ' ';
									std::cout << termcolor::reset << '\n';
								}
								return;
							}
						}
					}
				}
			}
		}
	}

	std::list<int> columns;
	for (int i = 0; i < 9; ++i)
	{
		nums.clear();
		columns.clear();
		for (int j = 0; j < 9; ++j)
		{
			if (counter[i][j] == 2 || counter[i][j] == 3)
				columns.push_back(j);
		}
		if (columns.size() >= 3)
		{
			for (auto column1 = columns.begin(); column1 != columns.end(); ++column1)
			{
				for (auto column2 = std::next(column1, 1); column2 != columns.end(); ++column2)
				{
					for (auto column3 = std::next(column2, 1); column3 != columns.end(); ++column3)
					{
						nums.clear();
						for (int n = 0; n < 9; ++n)
						{
							if ((nums.size() == 0 || std::find(nums.begin(), nums.end(), n) == nums.end()) && 
									(sudoku.sudoku_ans.cell[i][*column1].num[n]
									 || sudoku.sudoku_ans.cell[i][*column2].num[n]
									 || sudoku.sudoku_ans.cell[i][*column3].num[n]))
								nums.push_back(n);
						}
						if (nums.size() == 3)
						{
							for (int j = 0; j < 9; ++j)
							{
								if (std::find(columns.begin(), columns.end(), j) == columns.end() && sudoku.sudoku_ans.cell[j][i].done == false)
								{
									for (auto num : nums)
										disablePos(sudoku, num, i, j);
								}
							}
							if (sudoku.changed)
							{
								if (print_steps)
								{
									std::cout << termcolor::green << "Naked Triple (Row) : " << termcolor::magenta << 
										static_cast<char> (i + 65) << *column1 + 1 << ' ' << static_cast<char> (i + 65) << *column2 + 1 << ' ' <<
										static_cast<char> (i + 65) << *column3 + 1 << termcolor::reset << "removes " << termcolor::green;
									for (auto num : nums)
										std::cout << num + 1 << ' ';
									std::cout << termcolor::reset << '\n';
								}
								return;
							}
						}
					}
				}
			}
		}
	}

	POINT temp;
	std::list<POINT> positions;

	for (int i = 0; i <= 6; i += 3)
	{
		for (int j = 0; j <= 6; j += 3)
		{
			positions.clear();
			for (int k = i; k < i + 3; ++k)
			{
				for (int l = j; l < j + 3; ++l)
				{
					if (counter[k][l] == 2)
					{
						temp.x = k;
						temp.y = l;
						positions.push_back(temp);
					}
				}
			}
			if (positions.size() >= 3)
			{
				for (auto pos1 = positions.begin(); pos1 != positions.end(); ++pos1)
				{
					for (auto pos2 = std::next(pos1, 1); pos2 != positions.end(); ++pos2)
					{
						for (auto pos3 = std::next(pos2, 1); pos3 != positions.end(); ++pos3)
						{
							nums.clear();
							for (int n = 0; n < 9; ++n)
							{
								if ((nums.size() == 0 || std::find(nums.begin(), nums.end(), n) == nums.end()) && 
										(sudoku.sudoku_ans.cell[pos1->x][pos1->y].num[n]
										 || sudoku.sudoku_ans.cell[pos2->x][pos2->y].num[n]
										 || sudoku.sudoku_ans.cell[pos3->x][pos3->y].num[n]))
									nums.push_back(n);
							}
							if (nums.size() == 3)
							{
								for (int k = i; k < i + 3; ++k)
								{
									for (int l = j; l < j + 3; ++l)
									{
										if ((k != pos3->x || l != pos3->y) 
												&& (k != pos2->x || l != pos2->y) 
												&& (k != pos1->x || l != pos1->y) 
												&& sudoku.sudoku_ans.cell[k][l].done == false)
										{
											for (auto num : nums)
												disablePos(sudoku, num, k, l);
										}
									}
								}
								if (sudoku.changed)
								{
									if (print_steps)
									{
										std::cout << termcolor::green << "Naked Triple (Box) : " << termcolor::magenta;
										std::cout << static_cast<char> (pos1->x + 65) << pos1->y + 1 << ' '  << static_cast<char> (pos2->x + 65) 
											<< pos2->y + 1 << ' ' << static_cast<char> (pos3->x + 65) << pos3->y + 1 << termcolor::reset 
											<< " removes " << termcolor::green;
										for (auto num : nums)
											std::cout << num + 1 << ' ';
										std::cout << termcolor::reset << '\n';
									}
									return;
								}
							}
						}
					}
				}
			}
		}
	}
}

void CSudokuSolver::hiddenPair(SUDOKU &sudoku, bool print_steps = false)
{
	//TODO:check colums and boxes
	int counter[9][9];
	for (int i = 0; i < 9; ++i)
		for (int j = 0; j < 9; ++j)
			counter[i][j] = 0;

	for (int i = 0; i < 9; ++i)
		for (int j = 0; j < 9; ++j)
			for (int n = 0; n < 9; ++n)
				if (sudoku.sudoku_ans.cell[i][j].num[n])
					++counter[i][n];

	bool hidden_pair = true;
	int num1[2] = {-1, -1};
	int num2[2] = {-1, -1};
	for (int i = 0; i < 9; ++i)
		for (int j = 0; j < 9; ++j)
			if (counter[i][j] == 2)
			{
				for (int k = j + 1; k < 9; ++k)
				{
					if (counter[i][k] == 2)
					{
						num1[0] = j;
						num2[0] = k;
					}
				}
				if (num1[0] != -1 && num2[0] != -1)
				{
					for (int k = 0; k < 9; ++k) 
					{
						if ((sudoku.sudoku_ans.cell[i][k].num[num1[0]] && !sudoku.sudoku_ans.cell[i][k].num[num2[0]]) 
								|| (!sudoku.sudoku_ans.cell[i][k].num[num1[0]] && sudoku.sudoku_ans.cell[i][k].num[num2[0]]))
						{
							hidden_pair = false;
							break;
						}
						else if (sudoku.sudoku_ans.cell[i][k].num[num1[0]] && sudoku.sudoku_ans.cell[i][k].num[num2[0]])
						{
							num1[1] = k;
							for (int l = k + 1; l < 9; ++l)
							{
								if (sudoku.sudoku_ans.cell[i][l].num[num1[0]] && !sudoku.sudoku_ans.cell[i][l].num[num2[0]])
								{
									hidden_pair = false;
									break;
								}
								else if (!sudoku.sudoku_ans.cell[i][l].num[num1[0]] && sudoku.sudoku_ans.cell[i][l].num[num2[0]])
								{
									hidden_pair = false;
									break;
								}
								else if (sudoku.sudoku_ans.cell[i][l].num[num1[0]] && sudoku.sudoku_ans.cell[i][l].num[num2[0]])
								{
									num2[1] = l;
									break;
								}
							}
						}
					}
				}
				if (hidden_pair)
				{
					for (int k = 0; k < 9; ++k)
						if (k != num1[0] && k != num2[0])
						{
							disablePos(sudoku, k, i, num1[1]);
							disablePos(sudoku, k, i, num2[1]);
						}
					if (sudoku.changed)
						return;
				}
			}
}
void CSudokuSolver::pointingBoxRows(SUDOKU &sudoku, bool print_steps = false)
{
	//if a number exits in a 3x3 box such that it is only in one row
	//then that number must be only in that row
	//hence it may be disabled for the rest of the row
	bool only_row[9];

	for (int i = 0; i <= 6; i += 3)
		for (int j = 0; j <= 6; j += 3) 
		{
			for (auto& elem : only_row)
				elem = true;
			for (int k = i; k < i + 3 && k < 9; ++k)
				for (int l = j; l < j + 3 && l < 9; ++l) 
					if (sudoku.sudoku_ans.cell[k][l].done == false) 
						for (int n = 0; n < 9; ++n) 
							if (sudoku.sudoku_ans.cell[k][l].num[n] && only_row[n]) 
							{
								for (int m = i; m < i + 3 && m < 9; ++m)
									for (int o = j; o < j + 3 && o < 9; ++o) 
										if (m != k)
											if (sudoku.sudoku_ans.cell[m][o].done == false && sudoku.sudoku_ans.cell[m][o].num[n]) 
											{
												only_row[n] = false;
												break;
											}
								if (only_row[n])
								{
									for (int m = 0; m < j; ++m)
										disablePos(sudoku, n, k, m);
									for (int m = j + 3; m < 9; ++m)
										disablePos(sudoku, n, k, m);
									if (sudoku.changed)
									{
										if (print_steps)
											std::cout << termcolor::green << "Pointing Box (Rows) : " << termcolor::magenta << static_cast<char> (k + 65) 
												<< l + 1 << termcolor::reset << " removes " << termcolor::green << n + 1 << termcolor::reset << '\n';
										return;
									}
								}
							}

		}
}

void CSudokuSolver::pointingBoxColumns(SUDOKU &sudoku, bool print_steps = false)
{
	//if a number exits in a 3x3 box such that it is only in one column
	//then that number must be only in that column
	//hence it may be disabled for the rest of the column
	bool only_column[9];

	for (int i = 0; i <= 6; i += 3)
		for (int j = 0; j <= 6; j += 3) 
		{
			for (auto& elem : only_column)
				elem = true;
			for (int k = i; k < i + 3 && k < 9; ++k)
				for (int l = j; l < j + 3 && l < 9; ++l) 
					if (sudoku.sudoku_ans.cell[l][k].done == false) 
						for (int n = 0; n < 9; ++n)
							if (sudoku.sudoku_ans.cell[l][k].num[n] && only_column[n]) 
							{
								for (int m = i; m < i + 3; ++m)
									for (int o = j; o < j + 3; ++o) 
										if (m != k)
											if (sudoku.sudoku_ans.cell[o][m].done == false && sudoku.sudoku_ans.cell[o][m].num[n]) 
											{
												only_column[n] = false;
												break;
											}
								if (only_column[n])
								{
									for (int m = 0; m < j; ++m)
										disablePos(sudoku, n, m, k);
									for (int m = j + 3; m < 9; ++m)
										disablePos(sudoku, n, m, k);
									if (sudoku.changed)
									{
										if (print_steps)
											std::cout << termcolor::green << "Pointing Box (Columns) : " << termcolor::magenta << static_cast<char> (l + 65) 
												<< k + 1 << termcolor::reset << " removes " << termcolor::green << n + 1 << termcolor::reset << '\n';
										return;
									}
								}
							}
		}
}

void CSudokuSolver::boxLineReduceRow(SUDOKU &sudoku, bool print_steps = false)
{
	//if a number exists in a row such that it only occurs within a box,
	//that number may be disabled for the rest of the box
	bool only_box_row[9];
	for (int i = 0; i < 9; ++i)
	{
		for (auto& num : only_box_row)
			num = true;
		for (int j = 0; j < 9; ++j)
			if (sudoku.sudoku_ans.cell[i][j].done == false) 
				for (int n = 0; n < 9; ++n)
					if (sudoku.sudoku_ans.cell[i][j].num[n] && only_box_row[n])
					{
						for (int k = j - (j % 3) + 3; k < 9; ++k)
							if (sudoku.sudoku_ans.cell[i][k].num[n])
							{
								only_box_row[n] = false;
								break;
							}
						if (only_box_row[n])
						{
							for (int k = i - (i % 3); k < i - (i % 3) + 3 && k < 9; ++k)
								for (int l = j - (j % 3); l < j - (j % 3) + 3 && l < 9; ++l)
									if (k != i)
										disablePos(sudoku, n, k, l);
						}
						if (sudoku.changed)
						{
							if (print_steps)
								std::cout << termcolor::green << "Box Line Reduce (Row) : " << termcolor::magenta << static_cast<char> (i + 65) << j + 1 
									<< termcolor::reset << " removes " << termcolor::green << n + 1 << termcolor::reset << " for rest of box\n";
							return;
						}
					}
	}

}

void CSudokuSolver::boxLineReduceColumn(SUDOKU &sudoku, bool print_steps = false)
{
	//if a number exists in a column such that it only occurs within a box,
	//that number may be disabled for the rest of the box
	bool only_box_column[9];

	for (int i = 0; i < 9; ++i)
	{
		for (auto& num : only_box_column)
			num = true;
		for (int j = 0; j < 9; ++j)
			if (sudoku.sudoku_ans.cell[j][i].done == false) 
				for (int n = 0; n < 9; ++n)
					if (sudoku.sudoku_ans.cell[j][i].num[n] && only_box_column[n])
					{
						for (int k = j - (j % 3) + 3; k < 9; ++k)
							if (sudoku.sudoku_ans.cell[k][i].num[n])
							{
								only_box_column[n] = false;
								break;
							}
						if (only_box_column[n])
						{
							for (int k = i - (i % 3); k < i - (i % 3) + 3 && k < 9; ++k)
								for (int l = j - (j % 3); l < j - (j % 3) + 3 && l < 9; ++l)
									if (k != i)
										disablePos(sudoku, n, l, k);
						}
						if (sudoku.changed)
						{
							if (print_steps)
								std::cout << termcolor::green << "Box Line Reduce (Column) : " << termcolor::magenta << static_cast<char> (j + 65) << i + 1 
									<< termcolor::reset << " : removes " << termcolor::green << n + 1 << termcolor::reset << " for rest of box\n";
							return;
						}
					}
	}
}


void CSudokuSolver::xWing(SUDOKU &sudoku, bool print_steps = false)
{
	//if a number's possibilities form a rectangle
	//under certain conditions,
	//we may disable some of the numbers
	int counter[9][9];
	for (int i = 0; i < 9; ++i)
		for (int j = 0; j < 9; ++j)
			counter[i][j] = 0;

	for (int i = 0; i < 9; ++i)
		for (int j = 0; j < 9; ++j)
			if (sudoku.sudoku_ans.cell[i][j].done == false)
				for (int n = 0; n < 9; ++n)
					if (sudoku.sudoku_ans.cell[i][j].num[n]) 
						++counter[i][n];
	std::list<int> columns;

	for (int i = 0; i < 9; ++i)
	{
		for (int j = 0; j < 9; ++j)
		{
			if (counter[i][j] == 2)
			{
				for (int k = i + 1; k < 9; ++k)
					if (counter[k][j] == 2)
					{
						columns.clear();
						for (int l = 0; l < 9; ++l)
						{
							if ((columns.size() == 0 || std::find(columns.begin(), columns.end(), l) == columns.end()) 
									&& (sudoku.sudoku_ans.cell[i][l].num[j]
										|| sudoku.sudoku_ans.cell[k][l].num[j]))
								columns.push_back(l);
						}
						if (columns.size() == 2)
						{
							for (int l = 0; l < 9; ++l)
							{
								if (l != i && l != k)
									for (auto column : columns)
										disablePos(sudoku, j, l, column);
							}
							if (sudoku.changed)
							{
								if (print_steps)
								{
									std::cout << termcolor::cyan << "X-Wing (Rows) : " << termcolor::magenta;
									for (auto column : columns)
									{
										std::cout << static_cast<char> (i + 65) << column + 1 << ' ';
										std::cout << static_cast<char> (k + 65) << column + 1 << ' ';
									}
									std::cout << termcolor::reset << "removes " << termcolor::green << j + 1 << termcolor::reset << '\n';
								}
								return;
							}
						}
					}
			}
		}
	}

	for (int i = 0; i < 9; ++i)
		for (int j = 0; j < 9; ++j)
			counter[i][j] = 0;

	for (int i = 0; i < 9; ++i)
		for (int j = 0; j < 9; ++j)
			if (sudoku.sudoku_ans.cell[j][i].done == false)
				for (int n = 0; n < 9; ++n)
					if (sudoku.sudoku_ans.cell[j][i].num[n]) 
						++counter[n][i];
	std::list<int> rows;

	for (int i = 0; i < 9; ++i)
	{
		for (int j = 0; j < 9; ++j)
		{
			if (counter[j][i] == 2)
			{
				for (int k = i + 1; k < 9; ++k)
					if (counter[j][k] == 2)
					{
						rows.clear();
						for (int l = 0; l < 9; ++l)
						{
							if ((rows.size() == 0 || std::find(rows.begin(), rows.end(), l) == rows.end()) 
									&& (sudoku.sudoku_ans.cell[l][i].num[j]
										|| sudoku.sudoku_ans.cell[l][k].num[j]))
								rows.push_back(l);
						}
						if (rows.size() == 2)
						{
							for (int l = 0; l < 9; ++l)
							{
								if (l != i && l != k)
									for (auto row : rows)
										disablePos(sudoku, j, row, l);
							}
							if (sudoku.changed)
							{
								if (print_steps)
								{
									std::cout << termcolor::cyan << "X-Wing (Columns)" << termcolor::reset << " : " << termcolor::magenta;
									for (auto row : rows)
									{
										std::cout << static_cast<char> (row + 65) << i + 1 << ' ';
										std::cout << static_cast<char> (row + 65) << k + 1 << ' ';
									}
									std::cout << termcolor::reset << "removes " << termcolor::green << j + 1 << termcolor::reset << '\n';
								}
								return;
							}
						}
					}
			}
		}
	}
}

CSudokuSolver::POINT getPos3(CSudokuSolver::SUDOKU sudoku, CSudokuSolver::POINT pos1, CSudokuSolver::POINT pos2, int counter[9][9], std::list<int> nums)
{
	//get the third position for the y-wing function, give 2 positions
	//if none are found, return {-1, -1}
	std::list<int> nums_t;
	if (pos1.y == pos2.y)
	{
		for (int i = 0; i < 9; ++i)
			if (counter[pos1.x][i] == 2)
			{
				nums_t.clear();
				for (int n = 0; n < 9; ++n)
					if (sudoku.sudoku_ans.cell[pos1.x][i].num[n] || sudoku.sudoku_ans.cell[pos1.x][pos1.y].num[n])
						nums_t.push_back(n);
				CSudokuSolver::POINT temp = {pos1.x, i};
				if (nums_t == nums && numCommon(sudoku, pos2, temp) < 2 && numCommon(sudoku, pos1, temp) < 2)
					return temp;
			}
		for (int i = 0; i < 9; ++i)
		{
			if (counter[pos2.x][i] == 2)
			{
				nums_t.clear();
				for (int n = 0; n < 9; ++n)
					if (sudoku.sudoku_ans.cell[pos2.x][i].num[n] || sudoku.sudoku_ans.cell[pos2.x][pos2.y].num[n])
						nums_t.push_back(n);
				CSudokuSolver::POINT temp = {pos2.x, i};
				if (nums_t == nums && numCommon(sudoku, pos2, temp) < 2 && numCommon(sudoku, pos1, temp) < 2)
					return temp;
			}
		}
		for (int i = pos1.x - pos1.x % 3; i < pos1.x - (pos1.x % 3) + 3; ++i)
		{
			for (int j = pos1.y - pos1.y % 3; j < pos1.y - (pos1.y % 3) + 3; ++j)
			{
				if (j != pos1.y)
				{
					if (counter[i][j] == 2)
					{
						nums_t.clear();
						for (int n = 0; n < 9; ++n)
							if (sudoku.sudoku_ans.cell[i][j].num[n] || sudoku.sudoku_ans.cell[pos1.x][pos1.y].num[n])
								nums_t.push_back(n);
						CSudokuSolver::POINT temp = {i, j};
						if (nums_t == nums && numCommon(sudoku, pos2, temp) < 2 && (numCommon(sudoku, pos1, temp) < 2))
							return temp;
					}	
				}
			}
		}
		for (int i = pos2.x - pos2.x % 3; i < pos2.x - (pos2.x % 3) + 3; ++i)
		{
			for (int j = pos2.y - pos2.y % 3; j < pos2.y - (pos2.y % 3) + 3; ++j)
			{
				if (j != pos2.y)
				{
					if (counter[i][j] == 2)
					{
						nums_t.clear();
						for (int n = 0; n < 9; ++n)
							if (sudoku.sudoku_ans.cell[i][j].num[n] || sudoku.sudoku_ans.cell[pos2.x][pos2.y].num[n])
								nums_t.push_back(n);
						CSudokuSolver::POINT temp = {i, j};
						if (nums_t == nums && numCommon(sudoku, pos2, temp) < 2 && numCommon(sudoku, pos1, temp) < 2)
							return temp;
					}	
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < 9; ++i)
			if (counter[i][pos1.y] == 2)
			{
				nums_t.clear();
				for (int n = 0; n < 9; ++n)
					if (sudoku.sudoku_ans.cell[i][pos1.y].num[n] || sudoku.sudoku_ans.cell[pos1.x][pos1.y].num[n])
						nums_t.push_back(n);
				CSudokuSolver::POINT temp = {i , pos1.y};
				if (nums_t == nums && numCommon(sudoku, pos2, temp) < 2 && numCommon(sudoku, pos1, temp) < 2)
					return temp;
			}
		for (int i = 0; i < 9; ++i)
		{
			if (counter[i][pos2.y] == 2)
			{
				nums_t.clear();
				for (int n = 0; n < 9; ++n)
					if (sudoku.sudoku_ans.cell[i][pos2.y].num[n] || sudoku.sudoku_ans.cell[pos2.x][pos2.y].num[n])
						nums_t.push_back(n);
				CSudokuSolver::POINT temp = {i, pos2.y};
				if (nums_t == nums && numCommon(sudoku, pos2, temp) < 2 && numCommon(sudoku, pos1, temp) < 2)
					return temp;
			}
		}
		for (int i = pos1.y - pos1.y % 3; i < pos1.y - (pos1.y % 3) + 3; ++i)
		{
			for (int j = pos1.x - pos1.x % 3; j < pos1.x - (pos1.x % 3) + 3; ++j)
			{
				if (j != pos1.x)
				{
					if (counter[j][i] == 2)
					{
						nums_t.clear();
						for (int n = 0; n < 9; ++n)
							if (sudoku.sudoku_ans.cell[j][i].num[n] || sudoku.sudoku_ans.cell[pos1.x][pos1.y].num[n])
								nums_t.push_back(n);
						CSudokuSolver::POINT temp = {j, i};
						if (nums_t == nums && numCommon(sudoku, pos2, temp) < 2 && numCommon(sudoku, pos1, temp) < 2)
							return temp;
					}	
				}
			}
		}
		for (int i = pos2.y - pos2.y % 3; i < pos2.y - (pos2.y % 3) + 3; ++i)
		{
			for (int j = pos2.x - pos2.x % 3; j < pos2.x - (pos2.x % 3) + 3; ++j)
			{
				if (j != pos2.y)
				{
					if (counter[j][i] == 2)
					{
						nums_t.clear();
						for (int n = 0; n < 9; ++n)
							if (sudoku.sudoku_ans.cell[j][i].num[n] || sudoku.sudoku_ans.cell[pos2.x][pos2.y].num[n])
								nums_t.push_back(n);
						CSudokuSolver::POINT temp = {j, i};
						if (nums_t == nums && numCommon(sudoku, pos2, temp) < 2 && numCommon(sudoku, pos1, temp) < 2)
							return temp;
					}	
				}
			}
		}
	}

	return {-1,-1};
}

void CSudokuSolver::yWing(SUDOKU &sudoku, bool print_steps = false)
{
	//if the numbers occur in a certain pattern, 
	//under certain conditions, 
	//they may be disabled
	int counter[9][9];
	for (int i = 0; i < 9; ++i)
		for (int j = 0; j < 9; ++j)
			counter[i][j] = 0;

	for (int i = 0; i < 9; ++i)
		for (int j = 0; j < 9; ++j)
			if (sudoku.sudoku_ans.cell[i][j].done == false)
				for (int n = 0; n < 9; ++n)
					if (sudoku.sudoku_ans.cell[i][j].num[n]) 
						++counter[i][j];

	std::list<int> nums, nums_t;
	for (int i = 0; i < 9; ++i)
		for (int j = 0; j < 9; ++j)
			if (counter[i][j] == 2)
				for (int k = i - (i % 3) + 3; k < 9; ++k)
					if (counter[k][j] == 2)
					{
						nums.clear();
						for (int n = 0; n < 9; ++n)
							if ((nums.size() == 0 || std::find(nums.begin(), nums.end(), n) == nums.end()) 
									&& (sudoku.sudoku_ans.cell[i][j].num[n] 
										||  sudoku.sudoku_ans.cell[k][j].num[n]))
								nums.push_back(n);
						if (nums.size() == 3)
						{
							POINT pos1 = {i, j}, pos2 = {k , j};
							POINT pos3 = getPos3(sudoku, pos1, pos2, counter, nums);

							if (pos3.x == pos1.x || pos3.x - pos3.x % 3 == pos1.x - pos1.x % 3)
							{
								int common_num = getCommon(sudoku, pos2, pos3);
								if (common_num == -1 || pos3.x == -1)
									continue;

								disablePos(sudoku, common_num, pos2.x, pos3.y);
								if (pos2.y - pos2.y % 3 == pos3.y - pos3.y % 3)
									for (int l = pos2.x - pos2.x % 3; l < pos2.x - (pos2.x % 3) + 3; ++l)
										if (l != pos2.x)
											disablePos(sudoku, common_num, l, pos3.y);
								if (sudoku.changed)
								{
									if (print_steps)
									{
										std::cout << termcolor::cyan << "Y-Wing (Columns)" << termcolor::reset << " : " << termcolor::magenta 
											<< static_cast<char> (pos1.x + 65) << pos1.y + 1 << ' ' << static_cast<char> (pos2.x + 65) << pos2.y + 1 << ' ' 
											<< static_cast<char> (pos3.x + 65) << pos3.y + 1 << termcolor::reset << " containing " << termcolor::green;
										for (auto num : nums)
											std::cout << num + 1 << ' ';
										std::cout << termcolor::reset << "removes " << common_num + 1 << '\n';
									}
									return;
								}
							}
							else
							{
								int common_num = getCommon(sudoku, pos1, pos3);
								if (common_num == -1 || pos3.x == -1)
									continue;

								disablePos(sudoku, common_num, pos1.x, pos3.y);
								if (pos1.y - pos1.y % 3 == pos3.y - pos3.y % 3)
									for (int l = pos1.x - pos1.x % 3; l < pos1.x - (pos1.x % 3) + 3; ++l)
										disablePos(sudoku, common_num, l, pos3.y);
								if (sudoku.changed)
								{
									if (print_steps)
									{
										std::cout << termcolor::cyan << "Y-Wing (Columns)" << termcolor::reset << " : " << termcolor::magenta 
											<< static_cast<char> (pos1.x + 65) << pos1.y + 1 << ' ' << static_cast<char> (pos2.x + 65) << pos2.y + 1 << ' ' 
											<< static_cast<char> (pos3.x + 65) << pos3.y + 1 << termcolor::reset << " containing " << termcolor::green;
										for (auto num : nums)
											std::cout << num + 1 << ' ';
										std::cout << termcolor::reset << "removes " << common_num + 1 << '\n';
									}
									return;
								}

							}
						}
					}
	for (int i = 0; i < 9; ++i)
		for (int j = 0; j < 9; ++j)
			if (counter[j][i] == 2)
				for (int k = i - (i % 3) + 3; k < 9; ++k)
					if (counter[j][i] == 2)
					{
						nums.clear();
						for (int n = 0; n < 9; ++n)
							if ((nums.size() == 0 || std::find(nums.begin(), nums.end(), n) == nums.end()) 
									&& (sudoku.sudoku_ans.cell[j][i].num[n] 
										||  sudoku.sudoku_ans.cell[j][k].num[n]))
								nums.push_back(n);
						if (nums.size() == 3)
						{
							POINT pos1 = {j, i}, pos2 = {j , k};
							POINT pos3 = getPos3(sudoku, pos1, pos2, counter, nums);

							if (pos3.y == pos1.y || pos3.y - pos3.y % 3 == pos1.y - pos1.y % 3)
							{
								int common_num = getCommon(sudoku, pos2, pos3);
								if (common_num == -1 || pos3.x == -1)
									continue;
								disablePos(sudoku, common_num, pos3.x, pos2.y);
								if (pos2.x - pos2.x % 3 == pos3.x - pos3.x % 3)
									for (int l = pos2.y - pos2.y % 3; l < pos2.y - (pos2.y % 3) + 3; ++l)
										disablePos(sudoku, common_num, pos3.x, l);
								if (sudoku.changed)
								{
									if (print_steps)
									{
										std::cout << termcolor::cyan << "Y-Wing (Rows)" << termcolor::reset << " : " << termcolor::magenta 
											<< static_cast<char> (pos1.x + 65) << pos1.y + 1 << ' ' << static_cast<char> (pos2.x + 65) << pos2.y + 1 << ' ' 
											<< static_cast<char> (pos3.x + 65) << pos3.y + 1 << termcolor::reset << " containing " << termcolor::green;
										for (auto num : nums)
											std::cout << num + 1 << ' ';
										std::cout << termcolor::reset << "removes " << common_num + 1 << '\n';
									}
									return;
								}
							}
							else
							{
								int common_num = getCommon(sudoku, pos1, pos3);
								if (common_num == -1 || pos3.x == -1)
									continue;
								disablePos(sudoku, common_num, pos3.x, pos1.y);
								if (pos1.y - pos1.y % 3 == pos3.y - pos3.y % 3)
									for (int l = pos1.x - pos1.x % 3; l < pos1.x - (pos1.x % 3) + 3; ++l)
										disablePos(sudoku, common_num, l, pos3.y);
								if (sudoku.changed)
								{
									if (print_steps)
									{
										std::cout << termcolor::cyan << "Y-Wing (Rows)" << termcolor::reset << " : " << termcolor::magenta 
											<< static_cast<char> (pos1.x + 65) << pos1.y + 1 << ' ' << static_cast<char> (pos2.x + 65) << pos2.y + 1 << ' ' 
											<< static_cast<char> (pos3.x + 65) << pos3.y + 1 << termcolor::reset << " containing " << termcolor::green;
										for (auto num : nums)
											std::cout << num + 1 << ' ';
										std::cout << termcolor::reset << "removes " << common_num + 1 << '\n';
									}
									return;
								}

							}
						}
					}


}

void CSudokuSolver::trialError(SUDOKU &sudoku, bool print_steps)
{
	//use trial and error to solve the sudoku
	//try a number at a poition
	//if you reach a mistake,
	//that must not be its position
	//or if you reach the solution, great
	//else
	//try another number
	SUDOKU copy_sudoku = sudoku;
	for (int i = 0; i < 9; ++i)
		for (int j = 0; j < 9; ++j)
		{
			if (sudoku.sudoku_ans.cell[i][j].done == false)
				for (int n = 0; n < 9; ++n)
				{
					if (copy_sudoku.sudoku_ans.cell[i][j].num[n])
					{
						finalize(copy_sudoku, n, i, j);
						solveSudoku(copy_sudoku, /*print_steps = */ false, /*logical = */ true);
						if (checkError(copy_sudoku))
						{
							disablePos(sudoku, n, i, j);
							if (print_steps)
								std::cout << termcolor::green << "Trial and Error " << termcolor::reset << ": " << termcolor::magenta << static_cast<char> (i + 65) 
									<< j + 1 << termcolor::reset << ' ' << termcolor::green << n + 1 << termcolor::reset << " is not possible here\n";
							return;
						}
						else if (sudoku.num_solved == 81)
						{
							sudoku= copy_sudoku;
							if (print_steps)
								std::cout << termcolor::green << "Trial and Error " << termcolor::reset << ": Guessing " << termcolor::green << n + 1 << termcolor::reset
									<< " at " << termcolor::magenta << static_cast<char> (i + 65) << j + 1 << termcolor::reset << " -> Arrived at solution!\n";
							return;

						}
						else
						{
							copy_sudoku= sudoku;
						}
					}
				}
		}
}

void CSudokuSolver::solveSudoku(SUDOKU &sudoku, bool print_steps, bool logical)
{
	//Solve the sudokus using the above written algorithms
	while (sudoku.num_solved < 81 && sudoku.changed)
	{
		//get rid of the easy ones first
		sudoku.changed = false;
		nakedSingle(sudoku, print_steps);
		if (!sudoku.changed)
		{
			//Do the easy stuff now
			checkRows(sudoku, print_steps);
			checkColumns(sudoku, print_steps);
			checkBox(sudoku, print_steps);
		}
		if (!sudoku.changed)
		{
			if (sudoku.rating < 2)
				sudoku.rating = 2;
			nakedPair(sudoku, print_steps);		//Naked Pairs
		}
		if (!sudoku.changed)
			nakedTriple(sudoku, print_steps);	//Naked Triples
		if (!sudoku.changed)
		{
			//Pointing Box
			pointingBoxRows(sudoku, print_steps);
			pointingBoxColumns(sudoku, print_steps);
		}
		if (!sudoku.changed)
		{
			//Box Line Reduce
			boxLineReduceRow(sudoku, print_steps);
			boxLineReduceColumn(sudoku, print_steps);
		}
		if (!sudoku.changed)
		{
			if (sudoku.rating < 3)
				sudoku.rating = 3;
			xWing(sudoku, print_steps);		//X wing
		}
		if (!sudoku.changed)
		{
			if (sudoku.rating < 4)
				sudoku.rating = 4;
			yWing(sudoku, print_steps);		//Y wing
		}
		if (!sudoku.changed && !logical)
		{
			trialError(sudoku, print_steps);	//Trial Error
			sudoku.rating = 5;
		}
	}
}

bool CSudokuSolver::checkError(SUDOKU sudoku)
{
	//check for an error in the solution 
	//return true if there is an error
	//return false if there are no errors
	for (int i = 0; i < 9; ++i) 
		for (int j = 0; j < 9; ++j) 
			if (!sudoku.sudoku_a[i][j] && numPossible(sudoku, i, j) == 0) 
				return true;
	return false;
}

bool CSudokuSolver::bruteForce(SUDOKU &sudoku)
{
	solveSudoku(sudoku, false, true);
	if (checkError(sudoku))
		//return false on error in solution
		return false;
	if (sudoku.num_solved == 81)
		//return true if sudoku was solved
		return true;

	//create a copy to prevent issues
	SUDOKU copy_sudoku = sudoku;
	//get the position with least guesses to be made - less guesses = faster
	POINT least_pos = leastNumsPos(sudoku);

	for (int n = 0; n < 9; ++n)
		if (sudoku.sudoku_ans.cell[least_pos.x][least_pos.y].num[n])
		{
			//try the number a position
			finalize(copy_sudoku, n, least_pos.x, least_pos.y);
			if (!bruteForce(copy_sudoku))
				//the next bruteforce returned false, so this must be wrong solution
				copy_sudoku = sudoku;
			else
			{
				//this must be a right solution, so save it
				sudoku = copy_sudoku;
				return true;
			}
		}
	return false;
}
