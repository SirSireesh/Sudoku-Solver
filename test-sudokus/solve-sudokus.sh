#!/bin/bash

program=${BASH_SOURCE%/*}/../src/ssolver
testSudokus=${BASH_SOURCE%/*}/test.sudoku

line=0
for inp in $(<$testSudokus)
do
	let line=$line+1
	echo $inp | $program $* || (echo $line; exit 1)
done
exit 0
