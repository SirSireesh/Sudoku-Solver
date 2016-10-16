#Sudoku Solver
This is a sudoku solving program.

You should compile it with `clang -std=c++11 -O3 Sudoku-solver.cpp main.cpp -o Sudoku-solver` 

You need a compiler capable of c++11

###Supported Fuctions :
*Naked Singles/Pairs/Triples

*Hidden Singles

*Pointing Box Columns/Rows

*Box Line Reduction

###Usage
Sudoku-solver [options] 

The program accepts Sudokus from `stdin`. Blank boxes can be represented by spcaces, 0s or '.'s.

The options allowed are 

-h -- Print help menu

-t -- Print the method used to solve the Sudoku

-s -- Silently solve the sudoku and print unformatted output (i.e. output is like the input string)

-v -- Print version and exit
