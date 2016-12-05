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

* Y-Wing

* Trial and Error

* Brute Force

###Installing
You can run 

`aclocal` 

`automake --add-missing`

`autoreconf`

`./configure`

`make` 

`make install` 

in that order to install the program to `/opt/local/bin`

###Usage
ssolver [options] 

The program accepts Sudokus from `stdin`. Blank boxes can be represented by spaces, 0s or '.'s.

The options allowed are 

short | long  | Action|
------|-------|-------|
`-a`    |`--about`|Print license
`-b`  | `--brute-force`|Use Brute Force to solve the sudoku
`-h`    |`--help` |Print help menu
`-l`    |`--logical` |Use logic only to solve a sudoku (no trial and error)
`-t`    |`--trace`|Print the method used to solve the Sudoku
`-s` |`--silent`  |Silently solve the sudoku and print unformatted output (i.e. output is in a single line)
`-v` |`--version`|Print version and exit

If you installed using the above instructions, you can always run `man ssolver`
