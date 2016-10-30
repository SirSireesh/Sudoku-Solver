#!/bin/bash

program=${BASH_SOURCE%/*}/../bin/ssolver
testSudokus=${BASH_SOURCE%/*}/test.sudoku

echo "This program will solve all sudokus listed in test.sudoku using $program"

line=0
for inp in $(<$testSudokus)
do
	let line=$line+1
	echo $inp | $program $* || (echo $line; exit)
done
