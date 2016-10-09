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
#include "Sudoku-solver.h"

bool CSudokuSolver::initialiseSudoku(int sudoku_q[9][9], SUDOKU_ANS_BOARD &sudoku_ans)
{
	for (int i = 0; i < 9; ++i)
		for (int j = 0; j < 9; ++j) 
		{
			if (sudoku_ans.box[i][j].done == true || (sudoku_q[i][j] && sudoku_ans.box[i][j].num[sudoku_q[i][j] - 1] == false)) 
				return false;
			if (sudoku_q[i][j])
				finalize(sudoku_ans, sudoku_q, sudoku_q[i][j] - 1, i, j);	//finalize expects numbers in terms of array subscripts, hence num - 1
		}
	return true;
}

void CSudokuSolver::printSudoku(int sudoku_q[9][9])
{
	for (int i = 0; i < 9; ++i) 
	{
		for (int j = 0; j < 9; ++j)
			!(sudoku_q[i][j]) ? std::cout << " |" : std::cout << sudoku_q[i][j] << '|';
		std::cout << '\n';
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
		std::cout << "error : The number " << n + 1 << " was set to false at " << x << ' ' << y << "!\n";
		return;
	}
	if (sudoku_ans.box[x][y].done) 
	{
		std::cout << "error : The position " << x << ' ' << y << " was set to done!" << " Number " << sudoku_q[x][y] << " is already there!\n";
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

void CSudokuSolver::checkColumns(SUDOKU_ANS_BOARD &sudoku_ans, int sudoku_q[9][9])
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
							finalize(sudoku_ans, sudoku_q, n, j, i);
					}
	}
}

void CSudokuSolver::checkRows(SUDOKU_ANS_BOARD &sudoku_ans, int sudoku_q[9][9])
{
	bool only_pos[9];

	for (int i = 0; i < 9; ++i){
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
							finalize(sudoku_ans, sudoku_q, n, i, j);
					}
	}
}

void CSudokuSolver::checkBox(SUDOKU_ANS_BOARD &sudoku_ans, int sudoku_q[9][9])
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
									finalize(sudoku_ans, sudoku_q, n, k, l);
							}
		}
	}
}

void CSudokuSolver::nakedSingle(SUDOKU_ANS_BOARD &sudoku_ans, int sudoku_q[9][9])
{
	int poss = 0;
	int num;
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
				finalize(sudoku_ans, sudoku_q, num, i, j);
		}
}

void CSudokuSolver::nakedPair(SUDOKU_ANS_BOARD &sudoku_ans)
{
	//TODO:add column check
	int counter[9][9];
	bool pair = true;
	int num[2] = {-1, -1};
	for (int i = 0; i < 9; ++i) 
		for (int j = 0; j < 9; ++j) 
		{
			counter[i][j] = 0;
			if (sudoku_ans.box[i][j].done == false)
				for (int n = 0; n < 9; ++n)
					if (sudoku_ans.box[i][j].num[n])
						++counter[i][j];
		}

	for (int i = 0; i < 9; ++i)
	{
		for (int j = 0; j < 9; ++j)
		{	
			pair = true;
			if (counter[i][j] == 2) 
				for (int k = j + 1; k < 9; ++k) 
					if (counter[i][k] == 2 && sudoku_ans.box[i][k].done == false && sudoku_ans.box[i][j].done == false) 
					{
						for (int n = 0; n < 9; ++n) 
						{
							if (sudoku_ans.box[i][j].num[n] != sudoku_ans.box[i][k].num[n]) 
							{
								pair = false;
								break;
							}
							else if (sudoku_ans.box[i][j].num[n]) 
							{
								if (num[0] == -1)
									num[0] = n;
								else
									num[1] = n;
							}
						}
						if (pair) 
						{
							for (int l = 0; l < j; ++l)
							{
								disablePos(sudoku_ans, num[0], i, l);
								disablePos(sudoku_ans, num[1], i, l);
							}
							for (int l = j + 1; l < k; ++l) 
							{
								disablePos(sudoku_ans, num[0], i, l);
								disablePos(sudoku_ans, num[1], i, l);
							}
							for (int l = k + 1; l < 9; ++l) 
							{
								disablePos(sudoku_ans, num[0], i, l);
								disablePos(sudoku_ans, num[1], i, l);
							}

							if (j - (j % 3) == k - (k % 3)) 
								for (int l = i - (i % 3); l < (i - (i % 3) + 3); ++l) 
									for (int m = j - (j % 3); l < j - (j % 3) + 3; ++l)
										if (l == i && (m == j || m == k)) 
										{
											disablePos(sudoku_ans, num[0], l, m);
											disablePos(sudoku_ans, num[1], l, m);
										}
							if (sudoku_ans.changed)
								return;
						}
					}

		}
	}

	for (int i = 0; i < 9; ++i)
		for (int j = 0; j < 9; ++j) 
		{
			pair = true;
			if (counter[j][i] == 2)
				for (int k = j + 1; k < 9; ++k) 
					if (counter[k][i] == 2 && sudoku_ans.box[k][i].done == false && sudoku_ans.box[j][i].done == false) 
					{
						for (int n = 0; n < 9; ++n) 
						{
							if (sudoku_ans.box[j][i].num[n] != sudoku_ans.box[k][i].num[n]) 
							{
								pair = false;
								break;
							}
							else if (sudoku_ans.box[j][i].num[n]) 
							{
								if (num[0] == -1)
									num[0] = n;
								else
									num[1] = n;
							}
						}
						if (pair) 
						{
							for (int l = 0; l < j; ++l)
							{
								disablePos(sudoku_ans, num[0], l, i);
								disablePos(sudoku_ans, num[1], l, i);
							}
							for (int l = j + 1; l < k; ++l) 
							{
								disablePos(sudoku_ans, num[0], l, i);
								disablePos(sudoku_ans, num[1], l, i);
							}
							for (int l = k + 1; l < 9; ++l) 
							{
								disablePos(sudoku_ans, num[0], l, i);
								disablePos(sudoku_ans, num[1], l, i);
							}

							if (j - (j % 3) == k - (k % 3)) 
								//disable the box
								for (int l = i - (i % 3); l < (i - (i % 3) + 3); ++l) 
									for (int m = j - (j % 3); l < j - (j % 3) + 3; ++l)
										if (l == i && (m == j || m == k)) 
										{
											disablePos(sudoku_ans, num[0], l, m);
											disablePos(sudoku_ans, num[1], l, m);
										}
							if (sudoku_ans.changed)
								return;
						}
					}
		}
}


void CSudokuSolver::pointingBoxRows(SUDOKU_ANS_BOARD &sudoku_ans)
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
								}
							}
		}
}

void CSudokuSolver::pointingBoxColumns(SUDOKU_ANS_BOARD &sudoku_ans)
{
	bool only_column[9];

	for (int i = 0; i < 9; i += 3)
		for (int j = 0; j < 9; j += 3) 
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
								}
							}
		}
}

void CSudokuSolver::boxLineReduceRow(SUDOKU_ANS_BOARD &sudoku_ans)
{
	bool only_box_row[9];

	for (int i = 0; i < 9; ++i)
	{
		for (int j = 0; j < 9; ++j)
		{
			if (sudoku_ans.box[i][j].done == false) 
			{
				for (int n = 0; n < 9; ++n)
				{
					if (sudoku_ans.box[i][j].num[n] && only_box_row[n])
					{
						for (int k = j - (j % 3) + 3; k < 9; ++k)
						{
							if (sudoku_ans.box[i][k].num[n])
							{
								only_box_row[n] = false;
								break;
							}
						}
						if (only_box_row[n])
						{
							for (int k = i - (i % 3); k < i - (i % 3) + 3; ++k)
							{
								for (int l = j - (j % 3); l < j - (j % 3) + 3; ++l)
									if (k != i)
										disablePos(sudoku_ans, n, k, l);
							}
						}
						if (sudoku_ans.changed)
							return;
					}
				}
			}
		}
	}
}

void CSudokuSolver::boxLineReduceColumn(SUDOKU_ANS_BOARD &sudoku_ans)
{
	bool only_box_column[9];

	for (int i = 0; i < 9; ++i)
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
							for (int k = i - (i % 3); k < i - (i % 3) + 3; ++k)
								for (int l = j - (j % 3); l < j - (j % 3) + 3; ++l)
									if (k != i)
										disablePos(sudoku_ans, n, k, l);
						}
						if (sudoku_ans.changed)
							return;
					}
}

void CSudokuSolver::hiddenPair(SUDOKU_ANS_BOARD &sudoku_ans)
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

void CSudokuSolver::nakedTriple(SUDOKU_ANS_BOARD &sudoku_ans)
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
		if (rows.size() == 3)
		{
			for (auto row : rows)
			{
				for (int n = 0; n < 9; ++n)
				{
					if ((nums.size() == 0 || std::find(nums.begin(), nums.end(), n) == nums.end()) && sudoku_ans.box[row][i].num[n])
						nums.push_back(n);
				}
			}
			if (nums.size() == 3)
			{
				for (int j = 0; j < 9; ++j)
				{
					if (std::find(rows.begin(), rows.end(), j) == rows.end() && sudoku_ans.box[j][i].done == false)
					{
						for (auto num : nums)
							disablePos(sudoku_ans, num, j, i);
					}
				}
				if (sudoku_ans.changed)
					return;
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
		if (columns.size() == 3)
		{
			for (auto column : columns)
			{
				for (int n = 0; n < 9; ++n)
				{
					if ((nums.size() == 0 || std::find(nums.begin(), nums.end(), n) == nums.end()) && sudoku_ans.box[i][column].num[n])
						nums.push_back(n);
				}
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
					return;
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
					return true;
			}
	return false;
}
