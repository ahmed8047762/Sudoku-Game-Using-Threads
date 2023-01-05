# Sudoku Validator and Solver
This project is a sudoku validator and solver that uses threads and mutex for synchronization. It is capable of checking for duplicate entries in rows, columns, and 3x3 grids, and can also solve for missing numbers in a given sudoku puzzle.

## Features
* Validation of sudoku puzzle for duplicate entries in rows, columns, and 3x3 grids
* Solution of sudoku puzzle for missing numbers in rows, columns, and 3x3 grids
* Use of threads and mutex for synchronization
* Cancellation of threads using pthread_cancel() when necessary
## Usage
To use this project, you will need to call the following functions from the main() function:

* void *Column_Validation(void *args): Validates the sudoku puzzle for duplicate entries in columns.
* void *Row_Validation(void *args): Validates the sudoku puzzle for duplicate entries in rows.
* void *three_Grid_Validation(void *args): Validates the sudoku puzzle for duplicate entries in 3x3 grids.
* void *suduko_rowsolver(void *args): Solves for missing numbers in rows of the sudoku puzzle.
* void *suduko_colsolver(void *args): Solves for missing numbers in columns of the sudoku puzzle.
* void *suduko_3x3solver(void *args): Solves for missing numbers in the 3x3 grids of the sudoku puzzle.
* These functions can be called using the pthread_create() function. The pthread_cancel() function can be used to cancel threads as necessary.
