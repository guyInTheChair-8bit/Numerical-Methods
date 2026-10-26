# Linear System Solver

Despite the folder name, this project solves a linear system `A x = b`; it does not calculate a full matrix inverse. It offers two methods: Gaussian elimination and LU decomposition. Both methods work from an augmented matrix containing `A` and `b`.

## Input and methods

`augmented_matrix.txt` is a sample 3 by 4 augmented matrix. For a system with `N` unknowns, use `N` rows of `N + 1` whitespace-separated numbers. The last argument selects `Gaussian` or `LU` (case-sensitive).

## Build and run

```sh
gcc linear_system_solver.c -o linear_system_solver
./linear_system_solver augmented_matrix.txt 3 Gaussian
./linear_system_solver augmented_matrix.txt 3 LU
```

The program prints intermediate matrix information and the solution vector. It also reports when it cannot find a unique solution using the selected method.

## Limitations

The implementation checks for zero pivots but does not perform row pivoting. Consequently, some nonsingular systems may fail or produce inaccurate results. The sample matrix is provided to demonstrate the input format.
