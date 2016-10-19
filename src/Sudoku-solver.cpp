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
#include <list>
#include <algorithm>
#include <iterator>
#include <string>
#include "Sudoku-solver.h"
#include "termcolor.hpp"

bool CSudokuSolver::inputSudoku(int sudoku_q[9][9])
{
	std::string input;
	std::getline(std::cin, input);

	if (input.length() < 81)
	{
		std::cout << termcolor::red << "The input was too small! Check your input and try again!\n" << termcolor::reset;
		return false;
	}
	if (input.length() > 81)
	{
		std::cout << termcolor::red << "The input was too large! Check your input and try again!\n" << termcolor::reset;
		return false;
	}

	for (int i = 0; i < 9; ++i)
		for (int j = 0; j < 9; ++j)
		{
			switch (input[(i * 9) + j])
			{
				case '1': case '2' : case '3' : case '4' : case '5' : case '6' : case '7' : case '8' : case '9' :
					sudoku_q[i][j] = static_cast<int> (input[(i * 9) + j] - '0');
					break;
				case '0' : case '.' : case ' ' : 
					sudoku_q[i][j] = 0;
					break;
				default :
					std::cout << "Invalid input : " << input[(i * 9) + j] << "\n";
					return false;
			}
		}
	return true;
}

bool CSudokuSolver::initialiseSudoku(int sudoku_q[9][9], SUDOKU_ANS_BOARD &sudoku_ans)
{
	for (int i = 0; i < 9; ++i)
		for (int j = 0; j < 9; ++j) 
		{
			if (sudoku_ans.box[i][j].done == true || (sudoku_q[i][j] && sudoku_ans.box[i][j].num[sudoku_q[i][j] - 1] == false)) 
				return false;
			if (sudoku_q[i][j])
				finalize(sudoku_ans, sudoku_q, sudoku_q[i][j] - 1, i, j);
		}
	return true;
}

void CSudokuSolver::printSudoku(int sudoku_q[9][9])
{
	for (int i = 0; i < 9; ++i)
		for (int j = 0; j < 9; ++j)
			std::cout << sudoku_q[i][j];
	std::cout << '\n';
}

void CSudokuSolver::printfSudoku(int sudoku_q[9][9], int sudoku_a[9][9])
{
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
			if (!sudoku_q[i][j] && sudoku_a[i][j])
				std::cout << termcolor::red << sudoku_a[i][j] << termcolor::blue << " ";
			else if (!sudoku_q[i][j] && !sudoku_a[i][j])
				std::cout << termcolor::blue <<"  " << termcolor::reset;
			else if (sudoku_q[i][j])
				std::cout << termcolor::grey << sudoku_q[i][j] << termcolor::blue << " ";
		}
		std::cout << termcolor::reset << '\n';
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

void CSudokuSolver::disablePos(SUDOKU_ANS_BOARD &sudoku_ans, int n, int x, int y)
{
	if (sudoku_ans.box[x][y].num[n] == true && sudoku_ans.box[x][y].done == false) 
	{
		sudoku_ans.changed = true;
		sudoku_ans.box[x][y].num[n] = false;
	}	
}

void CSudokuSolver::disableColumn(SUDOKU_ANS_BOARD &sudoku_ans, int n, int column)
{
	for (int i = 0; i < 9; ++i) 
		if (sudoku_ans.box[i][column].done == false)
			disablePos(sudoku_ans, n, i, column);
}

void CSudokuSolver::disableRow(SUDOKU_ANS_BOARD &sudoku_ans, int n, int row)
{
	for (int j = 0; j < 9; ++j) 
		if (sudoku_ans.box[row][j].done == false)
			disablePos(sudoku_ans, n, row, j);
}

void CSudokuSolver::disableBox(SUDOKU_ANS_BOARD &sudoku_ans, int n, int x, int y)
{
	for (int i = x - (x%3); i < (x - (x%3) + 3); ++i)
		for (int j = y - (y%3); j < (y  - (y%3) + 3); ++j) 
			if (sudoku_ans.box[i][j].done == false)
				disablePos(sudoku_ans, n, i, j);
}

void CSudokuSolver::finalize(SUDOKU_ANS_BOARD &sudoku_ans, int sudoku_q[9][9], int n, int x, int y)
{
	if (sudoku_ans.box[x][y].num[n] == false) 
	{
		std::cout << termcolor::red << "error : The number " << n + 1 << " was set to false at " << x << ' ' << y << "!\n";
		return;
	}
	if (sudoku_ans.box[x][y].done) 
	{
		std::cout << termcolor::red << "error : The position " << x << ' ' << y << " was set to done!" << " Number " << sudoku_q[x][y] << " is already there!\n";
		return;
	}
	sudoku_ans.box[x][y].done = true;
	sudoku_ans.changed = true;
	sudoku_q[x][y] = n + 1;

	for (int i = 0; i < 9; ++i) 
		sudoku_ans.box[x][y].num[i] = false;

	disableRow(sudoku_ans, n, x);
	disableColumn(sudoku_ans, n, y);
	disableBox(sudoku_ans, n, x, y);
}

void CSudokuSolver::checkColumns(SUDOKU_ANS_BOARD &sudoku_ans, int sudoku_q[9][9], bool print_steps)
{
	bool only_pos[9];

	for (int i = 0; i < 9; ++i) 
	{
		for (auto& elem : only_pos)
			elem = true;
		for (int j = 0; j < 9; ++j) 
			if (sudoku_ans.box[j][i].done == false)
				for (int n = 0; n < 9; ++n)
					if (sudoku_ans.box[j][i].num[n] && only_pos[n]) 
					{
						for (int k = j + 1; k < 9; ++k)
							if (sudoku_ans.box[k][i].done == false && sudoku_ans.box[k][i].num[n]) 
							{
								only_pos[n] = false;
								break;
							}
						if (only_pos[n])
						{
							finalize(sudoku_ans, sudoku_q, n, j, i);
							if (print_steps)
								std::cout << termcolor::green << "Hidden Single : " << termcolor::magenta << static_cast<char> (j + 65) << i + 1 
									<< termcolor::reset << " set to " << termcolor::green << n + 1 << termcolor::reset << " : unique in column\n";
						}
					}
	}
}

void CSudokuSolver::checkRows(SUDOKU_ANS_BOARD &sudoku_ans, int sudoku_q[9][9], bool print_steps)
{
	bool only_pos[9];

	for (int i = 0; i < 9; ++i)
	{
		for (auto& elem : only_pos)		
			elem = true;
		for (int j = 0; j < 9; ++j) 
			if (sudoku_ans.box[i][j].done == false) 	
				for (int n  = 0; n < 9; ++n)
					if (sudoku_ans.box[i][j].num[n] && only_pos[n]) 
					{
						for (int k = j + 1; k < 9; ++k)
							if (sudoku_ans.box[i][k].done == false && sudoku_ans.box[i][k].num[n]) 
							{
								only_pos[n] = false;
								break;
							}
						if (only_pos[n]) 
						{
							finalize(sudoku_ans, sudoku_q, n, i, j);
							if (print_steps)
								std::cout << termcolor::green << "Hidden Single : " << termcolor::magenta << static_cast<char> (i + 65) << j + 1 
									<< termcolor::reset <<" set to " << termcolor::green << n + 1 << termcolor::reset << " : unique in row\n";
						}
					}
	}
}

void CSudokuSolver::checkBox(SUDOKU_ANS_BOARD &sudoku_ans, int sudoku_q[9][9], bool print_steps)
{
	bool only_pos[9];

	for (int i = 0; i < 9; i += 3) 
	{
		for (int j = 0; j < 9; j += 3) 
		{
			for (auto& elem : only_pos)
				elem = true;
			for (int k = i; k < i + 3 && k < 9; ++k) 
				for (int l = j; l < j + 3 && j < 9; ++l) 
					if (sudoku_ans.box[k][l].done == false)
						for (int n = 0; n < 9; ++n)
							if (sudoku_ans.box[k][l].num[n] && only_pos[n]) 
							{
								for (int m = i; m < i + 3; ++m) 
									for (int o = j; o < j + 3; ++o) 
										if (sudoku_ans.box[m][o].done == false) 
										{
											if ((m != k || o != l) && (sudoku_ans.box[m][o].num[n]))
											{
												only_pos[n] = false;
												break;
											}
										}
								if (only_pos[n])
								{
									finalize(sudoku_ans, sudoku_q, n, k, l);
									if (print_steps)
										std::cout << termcolor::green << "Hidden Single : " << termcolor::magenta << static_cast<char> (k + 65) << l + 1 
											<< termcolor::reset << " set to " << termcolor::green << n + 1 << termcolor::reset << " : unique in 3x3 box\n";
								}
							}
		}
	}
}

void CSudokuSolver::nakedSingle(SUDOKU_ANS_BOARD &sudoku_ans, int sudoku_q[9][9], bool print_steps)
{
	int poss = 0;
	int num = 0;
	for (int i = 0; i < 9; ++i)
		for (int j = 0; j < 9; ++j)
		{
			poss = 0;
			if (sudoku_ans.box[i][j].done == false)
				for (int n = 0; n < 9; ++n)
					if (sudoku_ans.box[i][j].num[n] == true) 
					{
						++poss;
						num = n;
					}
			if (poss == 1)
			{
				finalize(sudoku_ans, sudoku_q, num, i, j);
				if (print_steps)
					std::cout << termcolor::green << "Single : " << termcolor::magenta << static_cast<char> (i + 65) << j + 1 
						<< termcolor::reset << " set to " << termcolor::green << num + 1 <<  termcolor::reset << " : unique in box\n";
			}
		}
}

void CSudokuSolver::nakedPair(SUDOKU_ANS_BOARD &sudoku_ans, bool print_steps)
{
	int counter[9][9];
	for (int i = 0; i < 9; ++i)
		for (int j = 0; j < 9; ++j)
		{
			counter[j][i] = 0;
			if (sudoku_ans.box[j][i].done == false)
				for (int n = 0; n < 9; ++n)
					if (sudoku_ans.box[j][i].num[n])
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
			for (auto row1 : rows)
			{
				for (auto row2 : rows)
				{
					if (row2 > row1)
					{
						nums.clear();
						for (int n = 0; n < 9; ++n)
						{
							if ((nums.size() == 0 || std::find(nums.begin(), nums.end(), n) == nums.end()) && 
									(sudoku_ans.box[row1][i].num[n]
									 || sudoku_ans.box[row2][i].num[n]))
								nums.push_back(n);
						}
						if (nums.size() == 2)
						{
							for (int j = 0; j < 9; ++j)
							{
								if (j != row1 && j != row2 && sudoku_ans.box[j][i].done == false)
								{
									for (auto num : nums)
										disablePos(sudoku_ans, num, j, i);
								}
							}
							if (sudoku_ans.changed)
							{
								if (print_steps)
								{
									std::cout << termcolor::green << "Naked Pair (Column) : " << termcolor::magenta;
									std::cout << static_cast<char> (row1 + 65) << i + 1 << ' ' << static_cast<char> (row2 + 65) << i + 1
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
			for (auto column1 : columns)
			{
				for (auto column2 : columns)
				{
					if (column2 > column1)
					{
						nums.clear();
						for (int n = 0; n < 9; ++n)
						{
							if ((nums.size() == 0 || std::find(nums.begin(), nums.end(), n) == nums.end()) && 
									(sudoku_ans.box[i][column1].num[n]
									 || sudoku_ans.box[i][column2].num[n]))
								nums.push_back(n);
						}
						if (nums.size() == 2)
						{
							for (int j = 0; j < 9; ++j)
							{
								if (std::find(columns.begin(), columns.end(), j) == columns.end() && sudoku_ans.box[i][j].done == false)
								{
									for (auto num : nums)
										disablePos(sudoku_ans, num, i, j);
								}
							}
							if (sudoku_ans.changed)
							{
								if (print_steps)
								{
									std::cout << termcolor::green << "Naked Pair (Row) : " << termcolor::magenta;
									std::cout << static_cast<char> (i + 65) << column1 + 1 << ' ' << static_cast<char> (i + 65) << column2 + 1
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

	struct POINT
	{
		int x;
		int y;
	};
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
				for (auto pos1 : positions)
				{
					for (auto pos2 : positions)
					{
						if (pos2.x > pos1.x || pos2.y > pos1.y)
						{
							nums.clear();
							for (int n = 0; n < 9; ++n)
							{
								if ((nums.size() == 0 || std::find(nums.begin(), nums.end(), n) == nums.end()) && 
										(sudoku_ans.box[pos1.x][pos1.y].num[n]
										 || sudoku_ans.box[pos2.x][pos2.y].num[n]))
									nums.push_back(n);
							}
							if (nums.size() == 2)
							{
								for (int k = i; k < i + 3; ++k)
								{
									for (int l = j; l < j + 3; ++l)
									{
										if ((k != pos2.x || l != pos2.y) && (k != pos1.x || l != pos1.y) && sudoku_ans.box[k][l].done == false)
										{
											for (auto num : nums)
												disablePos(sudoku_ans, num, k, l);
										}
									}
								}
								if (sudoku_ans.changed)
								{
									if (print_steps)
									{
										std::cout << termcolor::green << "Naked Pair (Box) : " << termcolor::magenta;
										std::cout << static_cast<char> (pos1.x + 65) << pos1.y + 1 << ' ' << static_cast<char> (pos2.x + 65) << pos2.y + 1
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
}

void CSudokuSolver::pointingBoxRows(SUDOKU_ANS_BOARD &sudoku_ans, bool print_steps)
{
	bool only_row[9];

	for (int i = 0; i <= 6; i += 3)
		for (int j = 0; j <= 6; j += 3) 
		{
			for (auto& elem : only_row)
				elem = true;
			for (int k = i; k < i + 3 && k < 9; ++k)
				for (int l = j; l < j + 3 && l < 9; ++l) 
					if (sudoku_ans.box[k][l].done == false) 
						for (int n = 0; n < 9; ++n) 
							if (sudoku_ans.box[k][l].num[n] && only_row[n]) 
							{
								for (int m = i; m < i + 3 && m < 9; ++m)
									for (int o = j; o < j + 3 && o < 9; ++o) 
										if (m != k)
											if (sudoku_ans.box[m][o].done == false && sudoku_ans.box[m][o].num[n]) 
											{
												only_row[n] = false;
												break;
											}
								if (only_row[n])
								{
									for (int m = 0; m < j; ++m)
										disablePos(sudoku_ans, n, k, m);
									for (int m = j + 3; m < 9; ++m)
										disablePos(sudoku_ans, n, k, m);
									if (sudoku_ans.changed)
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

void CSudokuSolver::pointingBoxColumns(SUDOKU_ANS_BOARD &sudoku_ans, bool print_steps)
{
	bool only_column[9];

	for (int i = 0; i <= 6; i += 3)
		for (int j = 0; j <= 6; j += 3) 
		{
			for (auto& elem : only_column)
				elem = true;
			for (int k = i; k < i + 3 && k < 9; ++k)
				for (int l = j; l < j + 3 && l < 9; ++l) 
					if (sudoku_ans.box[l][k].done == false) 
						for (int n = 0; n < 9; ++n)
							if (sudoku_ans.box[l][k].num[n] && only_column[n]) 
							{
								for (int m = i; m < i + 3; ++m)
									for (int o = j; o < j + 3; ++o) 
										if (m != k)
											if (sudoku_ans.box[o][m].done == false && sudoku_ans.box[o][m].num[n]) 
											{
												only_column[n] = false;
												break;
											}
								if (only_column[n])
								{
									for (int m = 0; m < j; ++m)
										disablePos(sudoku_ans, n, m, k);
									for (int m = j + 3; m < 9; ++m)
										disablePos(sudoku_ans, n, m, k);
									if (sudoku_ans.changed)
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

void CSudokuSolver::boxLineReduceRow(SUDOKU_ANS_BOARD &sudoku_ans, bool print_steps)
{
	bool only_box_row[9];
	for (int i = 0; i < 9; ++i)
	{
		for (auto& num : only_box_row)
			num = true;
		for (int j = 0; j < 9; ++j)
			if (sudoku_ans.box[i][j].done == false) 
				for (int n = 0; n < 9; ++n)
					if (sudoku_ans.box[i][j].num[n] && only_box_row[n])
					{
						for (int k = j - (j % 3) + 3; k < 9; ++k)
							if (sudoku_ans.box[i][k].num[n])
							{
								only_box_row[n] = false;
								break;
							}
						if (only_box_row[n])
						{
							for (int k = i - (i % 3); k < i - (i % 3) + 3 && k < 9; ++k)
								for (int l = j - (j % 3); l < j - (j % 3) + 3 && l < 9; ++l)
									if (k != i)
										disablePos(sudoku_ans, n, k, l);
						}
						if (sudoku_ans.changed)
						{
							if (print_steps)
								std::cout << termcolor::green << "Box Line Reduce (Row) : " << termcolor::magenta << static_cast<char> (i + 65) << j + 1 
									<< termcolor::reset << " removes " << termcolor::green << n + 1 << termcolor::reset << " for rest of box\n";
							return;
						}
					}
	}

}

void CSudokuSolver::boxLineReduceColumn(SUDOKU_ANS_BOARD &sudoku_ans, bool print_steps)
{
	bool only_box_column[9];

	for (int i = 0; i < 9; ++i)
	{
		for (auto& num : only_box_column)
			num = true;
		for (int j = 0; j < 9; ++j)
			if (sudoku_ans.box[j][i].done == false) 
				for (int n = 0; n < 9; ++n)
					if (sudoku_ans.box[j][i].num[n] && only_box_column[n])
					{
						for (int k = j - (j % 3) + 3; k < 9; ++k)
							if (sudoku_ans.box[k][i].num[n])
							{
								only_box_column[n] = false;
								break;
							}
						if (only_box_column[n])
						{
							for (int k = i - (i % 3); k < i - (i % 3) + 3 && k < 9; ++k)
								for (int l = j - (j % 3); l < j - (j % 3) + 3 && l < 9; ++l)
									if (k != i)
										disablePos(sudoku_ans, n, l, k);
						}
						if (sudoku_ans.changed)
						{
							if (print_steps)
								std::cout << termcolor::green << "Box Line Reduce (Column) : " << termcolor::magenta << static_cast<char> (j + 65) << i + 1 
									<< termcolor::reset << " : removes " << termcolor::green << n + 1 << termcolor::reset << " for rest of box\n";
							return;
						}
					}
	}
}

void CSudokuSolver::hiddenPair(SUDOKU_ANS_BOARD &sudoku_ans, bool print_steps)
{
	//TODO:check colums and boxes
	int counter[9][9];
	for (int i = 0; i < 9; ++i)
		for (int j = 0; j < 9; ++j)
			counter[i][j] = 0;

	for (int i = 0; i < 9; ++i)
		for (int j = 0; j < 9; ++j)
			for (int n = 0; n < 9; ++n)
				if (sudoku_ans.box[i][j].num[n])
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
						if ((sudoku_ans.box[i][k].num[num1[0]] && !sudoku_ans.box[i][k].num[num2[0]]) || (!sudoku_ans.box[i][k].num[num1[0]] && sudoku_ans.box[i][k].num[num2[0]]))
						{
							hidden_pair = false;
							break;
						}
						else if (sudoku_ans.box[i][k].num[num1[0]] && sudoku_ans.box[i][k].num[num2[0]])
						{
							num1[1] = k;
							for (int l = k + 1; l < 9; ++l)
							{
								if (sudoku_ans.box[i][l].num[num1[0]] && !sudoku_ans.box[i][l].num[num2[0]])
								{
									hidden_pair = false;
									break;
								}
								else if (!sudoku_ans.box[i][l].num[num1[0]] && sudoku_ans.box[i][l].num[num2[0]])
								{
									hidden_pair = false;
									break;
								}
								else if (sudoku_ans.box[i][l].num[num1[0]] && sudoku_ans.box[i][l].num[num2[0]])
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
							disablePos(sudoku_ans, k, i, num1[1]);
							disablePos(sudoku_ans, k, i, num2[1]);
						}
					if (sudoku_ans.changed)
						return;
				}
			}
}

void CSudokuSolver::nakedTriple(SUDOKU_ANS_BOARD &sudoku_ans, bool print_steps)
{
	int counter[9][9];
	for (int i = 0; i < 9; ++i)
		for (int j = 0; j < 9; ++j)
		{
			counter[j][i] = 0;
			if (sudoku_ans.box[j][i].done == false)
				for (int n = 0; n < 9; ++n)
					if (sudoku_ans.box[j][i].num[n])
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
									(sudoku_ans.box[*row1][i].num[n]
									 || sudoku_ans.box[*row2][i].num[n]
									 || sudoku_ans.box[*row3][i].num[n]))
								nums.push_back(n);
						}
						if (nums.size() == 3)
						{
							for (int j = 0; j < 9; ++j)
							{
								if (j != *row1 && j != *row2 && j != *row3 && sudoku_ans.box[i][j].done == false)
								{
									for (auto num : nums)
										disablePos(sudoku_ans, num, j, i);
								}
							}
							if (sudoku_ans.changed)
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
									(sudoku_ans.box[i][*column1].num[n]
									 || sudoku_ans.box[i][*column2].num[n]
									 || sudoku_ans.box[i][*column3].num[n]))
								nums.push_back(n);
						}
						if (nums.size() == 3)
						{
							for (int j = 0; j < 9; ++j)
							{
								if (std::find(columns.begin(), columns.end(), j) == columns.end() && sudoku_ans.box[j][i].done == false)
								{
									for (auto num : nums)
										disablePos(sudoku_ans, num, i, j);
								}
							}
							if (sudoku_ans.changed)
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

	struct POINT
	{
		int x;
		int y;
	};
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
										(sudoku_ans.box[pos1->x][pos1->y].num[n]
										 || sudoku_ans.box[pos2->x][pos2->y].num[n]
										 || sudoku_ans.box[pos3->x][pos3->y].num[n]))
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
												&& sudoku_ans.box[k][l].done == false)
										{
											for (auto num : nums)
												disablePos(sudoku_ans, num, k, l);
										}
									}
								}
								if (sudoku_ans.changed)
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

void CSudokuSolver::xWing(SUDOKU_ANS_BOARD &sudoku_ans, bool print_steps)
{
	int counter[9][9];
	for (int i = 0; i < 9; ++i)
		for (int j = 0; j < 9; ++j)
			counter[i][j] = 0;

	for (int i = 0; i < 9; ++i)
		for (int j = 0; j < 9; ++j)
			if (sudoku_ans.box[i][j].done == false)
				for (int n = 0; n < 9; ++n)
					if (sudoku_ans.box[i][j].num[n]) 
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
									&& (sudoku_ans.box[i][l].num[j]
										|| sudoku_ans.box[k][l].num[j]))
								columns.push_back(l);
						}
						if (columns.size() == 2)
						{
							for (int l = 0; l < 9; ++l)
							{
								if (l != i && l != k)
									for (auto column : columns)
										disablePos(sudoku_ans, j, l, column);
							}
							if (sudoku_ans.changed)
							{
								if (print_steps)
								{
									std::cout << termcolor::green << "X Wing (Rows) : " << termcolor::magenta;
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
			if (sudoku_ans.box[j][i].done == false)
				for (int n = 0; n < 9; ++n)
					if (sudoku_ans.box[j][i].num[n]) 
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
									&& (sudoku_ans.box[l][i].num[j]
										|| sudoku_ans.box[l][k].num[j]))
								rows.push_back(l);
						}
						if (rows.size() == 2)
						{
							for (int l = 0; l < 9; ++l)
							{
								if (l != i && l != k)
									for (auto row : rows)
										disablePos(sudoku_ans, j, row, l);
							}
							if (sudoku_ans.changed)
							{
								if (print_steps)
								{
									std::cout << termcolor::green << "X Wing (Columns) : " << termcolor::magenta;
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

void CSudokuSolver::trialError(SUDOKU_ANS_BOARD &sudoku_ans, int sudoku_a[9][9], bool print_steps)
{
	int copy_sudoku_a[9][9];
	SUDOKU_ANS_BOARD copy_sudoku_ans = sudoku_ans;
	for (int i = 0; i < 9; ++i)
		for (int j = 0; j < 9; ++j)
			copy_sudoku_a[i][j] = sudoku_a[i][j];
	for (int i = 0; i < 9; ++i)
		for (int j = 0; j < 9; ++j)
		{
			if (copy_sudoku_ans.box[i][j].done == false)
				for (int n = 0; n < 9; ++n)
				{
					if (copy_sudoku_ans.box[i][j].num[n])
					{
						finalize(copy_sudoku_ans, copy_sudoku_a, n, i, j);
						while (count(copy_sudoku_a) < 81 && copy_sudoku_ans.changed) 
						{
							copy_sudoku_ans.changed = false;
							checkColumns(copy_sudoku_ans, copy_sudoku_a, false);
							checkRows(copy_sudoku_ans, copy_sudoku_a, false);
							nakedSingle(copy_sudoku_ans, copy_sudoku_a, false);
							checkBox(copy_sudoku_ans, copy_sudoku_a, false);
							if (!copy_sudoku_ans.changed) 
								boxLineReduceRow(copy_sudoku_ans, false);
							if (!copy_sudoku_ans.changed)
								boxLineReduceColumn(copy_sudoku_ans, false);
							if (!copy_sudoku_ans.changed)
								pointingBoxColumns(copy_sudoku_ans, false);
							if (!copy_sudoku_ans.changed)
								pointingBoxRows(copy_sudoku_ans, false);
							if (!copy_sudoku_ans.changed)
								nakedPair(copy_sudoku_ans, false);
							if (!copy_sudoku_ans.changed)
								nakedTriple(copy_sudoku_ans, false);
							if (!copy_sudoku_ans.changed)
								xWing(copy_sudoku_ans, false);
						}
						if (checkError(copy_sudoku_ans, copy_sudoku_a))
						{
							disablePos(sudoku_ans, n, i, j);
							if (print_steps)
								std::cout << termcolor::green << "Trial and Error " << termcolor::reset << ": " << termcolor::magenta << static_cast<char> (i + 65) 
									<< j + 1 << termcolor::reset << ' ' << termcolor::green << n + 1 << termcolor::reset << " is not possible here\n";
							return;
						}
						else if (count(copy_sudoku_a) == 81)
						{
							sudoku_ans = copy_sudoku_ans;
							for (int i = 0; i < 9; ++i)
								for (int j = 0; j < 9; ++j)
									sudoku_a[i][j] = copy_sudoku_a[i][j];
							if (print_steps)
								std::cout << termcolor::green << "Trial and Error " << termcolor::reset << ": Guessing " << termcolor::green << n + 1 << termcolor::reset
									<< " at " << termcolor::magenta << static_cast<char> (i + 65) << j + 1 << termcolor::reset << "-> Arrived at solution!\n";
							return;

						}
						else
						{
							copy_sudoku_ans = sudoku_ans;
							for (int i = 0; i < 9; ++i)
								for (int j = 0; j < 9; ++j)
									copy_sudoku_a[i][j] = sudoku_a[i][j];
						}
					}
				}
		}
}

bool CSudokuSolver::checkError(SUDOKU_ANS_BOARD &sudoku_ans, int sudoku_q[9][9])
{
	int poss = 0;
	for (int i = 0; i < 9; ++i) 
		for (int j = 0; j < 9; ++j) 
			if (!sudoku_q[i][j]) 
			{
				poss = 0;
				for (int n = 0; n < 9; ++n)
					if (sudoku_ans.box[i][j].num[n])
						++poss;	
				if (poss == 0)
				{
					return true;
				}
			}
	return false;
}
