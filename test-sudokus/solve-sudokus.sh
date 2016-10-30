#!/bin/bash
program=${BASH_SOURCE%/*}/../bin/ssolver
testSudokus=${BASH_SOURCE%/*}/test.sudoku

echo "This program will solve all sudokus listed in test.sudoku using $program"

line=0
while read in; do 
	(echo $in | $program $* && line += 1) || echo $line && exit;
done < "$testSudokus"
