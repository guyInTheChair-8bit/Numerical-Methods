# Gaussian Elimination

Reads an augmented matrix from a text file, performs forward elimination and back substitution, then prints the solution vector. `gaussian_elimination_algorithm.txt` describes the algorithm. Supply a text file with N rows and N+1 whitespace-separated values per row.

```sh
gcc gaussian_elimination.c -o gaussian_elimination
./gaussian_elimination path/to/augmented_matrix.txt 3
```
