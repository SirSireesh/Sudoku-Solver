#Sudoku Solver
This is a sudoku solving program.

You should compile it with `clang -std=c++11 -O3 -march=native Sudoku-solver.cpp main.cpp -o ssolver` 

Note : gcc users must compile with `g++ -std=gnu++11 -O3 -march=native Sudoku-solver.cpp main.cpp -o ssolver`

You need a compiler capable of c++11

###Supported Fuctions :
* Naked Singles/Pairs/Triples

* Hidden Singles

* Pointing Box Columns/Rows

* Box Line Reduction

* X-Wing

* Trial and Error

###Usage
ssolver [options] 

The program accepts Sudokus from `stdin`. Blank boxes can be represented by spaces, 0s or '.'s.

The options allowed are 

-a -- print license

-h -- Print help menu

-l -- Use logic only to solve a sudoku (no trial and error)

-t -- Print the method used to solve the Sudoku

-s -- Silently solve the sudoku and print unformatted output (i.e. output is in a single line)

-v -- Print version and exit
