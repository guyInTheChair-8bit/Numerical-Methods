# Gaussian Elimination

This program solves a square linear system by reading its augmented matrix, applying forward elimination, and using back substitution to calculate the solution vector. The implementation uses dynamically allocated arrays and reports the solution in the terminal.

## Input format

For a system with `N` unknowns, provide a text file with `N` rows and `N + 1` whitespace-separated numeric values on each row. The final value in each row is the right-hand-side constant. The repository includes `gaussian_elimination_algorithm.txt` with pseudocode; provide your own matrix data file when running the program.

## Build and run

```sh
gcc gaussian_elimination.c -o gaussian_elimination
./gaussian_elimination path/to/augmented_matrix.txt 3
```

The second argument is the number of equations and must match the input file. The program prints the matrix and the resulting `X[i]` values.

## Limitations

This implementation does not pivot rows. A zero pivot can cause an invalid result, so it is unsuitable for matrices that require pivoting or careful numerical conditioning. For a solver with a second method, see [Matrix Inversion](../Matrix%20Inversion/README.md).
