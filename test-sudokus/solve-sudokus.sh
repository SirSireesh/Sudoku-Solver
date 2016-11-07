#!/bin/bash

program=${BASH_SOURCE%/*}/../bin/ssolver
testSudokus=${BASH_SOURCE%/*}/test.sudoku

line=0
for inp in $(<$testSudokus)
do
	let line=$line+1
	echo $inp | $program $* || (echo $line; exit 1)
done
exit 0
