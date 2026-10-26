#include <stdio.h>
#include <stdlib.h> 

void printMatrix(double **M, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%8.2f ", M[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    // create a 2d array called "M" of size N x N+1
    double **M;
    
    // create another matrix: X[0 .. N-1] to store the final solutions
    double *X;
    
    double factor;

    if (argc != 3) {
        printf("Usage: %s <filename> <N>\n", argv[0]);
        printf("  <filename>: Path to the matrix data file.\n");
        printf("  <N>: The number of equations (rows) in the system.\n");
        exit(1);
    }

    int N = atoi(argv[2]); 
    if (N <= 0) {
        printf("Error: Invalid value for N (%s). N must be a positive integer.\n", argv[2]);
        exit(1);
    }
    printf("N = %d equations (from command line).\n", N);

    // --- File Reading ---
    FILE *file;
    file = fopen(argv[1], "r");

    if (file == NULL) {
        printf("Error: Could not open file '%s'.\n", argv[1]);
        exit(1); // Exit the program if file can't be opened
    }

    // --- Dynamic Memory Allocation ---
    // Allocate X (1D array for solutions)
    X = (double *)malloc(N * sizeof(double));
    if (X == NULL) {
        printf("Error: Memory allocation failed for X.\n");
        fclose(file);
        exit(1);
    }

    // Allocate M (array of N pointers to double)
    M = (double **)malloc(N * sizeof(double *));
    if (M == NULL) {
        printf("Error: Memory allocation failed for M (rows).\n");
        free(X);
        fclose(file);
        exit(1);
    }
    
    // Allocate each row of M (N+1 doubles per row)
    for (int i = 0; i < N; i++) {
        M[i] = (double *)malloc((N + 1) * sizeof(double));
        if (M[i] == NULL) {
            printf("Error: Memory allocation failed for M (column %d).\n", i);
            // Free already allocated memory
            for (int j = 0; j < i; j++) {
                free(M[j]);
            }
            free(M);
            free(X);
            fclose(file);
            exit(1);
        }
    }

    printf("Reading augmented matrix from '%s'...\n", argv[1]);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N + 1; j++) {
            // Read one double value. If it fails, print an error.
            if (fscanf(file, "%lf", &M[i][j]) != 1) {
                printf("Error reading data from file at row %d, col %d.\n", i, j);
                printf("Check if file has enough data for N=%d rows.\n", N);
                // (Proper cleanup would go here too, but exit for simplicity)
                fclose(file);
                // Free memory before exiting
                for (int k = 0; k < N; k++) free(M[k]);
                free(M);
                free(X);
                exit(1);
            }
        }
    }
    fclose(file); // Close the file after reading

    printf("Initial Matrix:\n");
    printMatrix(M, N, N + 1);

    // --- Forward Elimination ---
    for (int p = 0; p < N - 1; p++) {
        if (M[p][p] == 0.0) {
            printf("\nError: Zero pivot element at M[%d][%d].\n", p, p);
            printf("The matrix may be singular. Pivoting is required.\n");
            
            // Cleanup memory before exiting
            for (int i = 0; i < N; i++) free(M[i]);
            free(M);
            free(X);
            exit(1);
        }

        for (int i = p + 1; i < N; i++) {
            // Calculate the factor once per row 'i' for efficiency
            factor = M[i][p] / M[p][p]; 
            
            // This loop is slightly inefficient (starts at j=0)
            for (int j = 0; j < N + 1; j++) {
                M[i][j] = M[i][j] - factor * M[p][j];
            }
        }
    }

    printf("\nUpper Triangular Matrix:\n");
    printMatrix(M, N, N + 1);

    // --- Back Substitution ---
    for (int i = N - 1; i >= 0; i--) {
        X[i] = M[i][N]; // Start with the constant term
        for (int j = N - 1; j > i; j--) {
            X[i] -= M[i][j] * X[j]; // Subtract known values
        }
        X[i] = X[i] / M[i][i]; // Divide by the diagonal element
    }

    // --- Print Solution ---
    printf("\nSolution:\n");
    for (int i = 0; i < N; i++) {
        printf("X[%d] = %f\n", i, X[i]);
    }

    for (int i = 0; i < N; i++) {
        free(M[i]); 
    }
    free(M); 
    free(X);
    return 0;
}


