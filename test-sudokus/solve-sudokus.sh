#!/bin/bash
program=~/Documents/School/11B/Computers/Sudoku-Solver/bin/Sudoku-solver
testSudokus=~/Documents/School/11B/Computers/Sudoku-Solver/test-sudokus/test.sudoku

echo "This program will solve all sudokus listed in test.sudoku using $program"

while read in; do echo $in | $program $*; done < "$testSudokus"
