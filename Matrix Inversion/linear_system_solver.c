#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 

void printMatrix(double **M, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%8.2f ", M[i][j]);
        }
        printf("\n");
    }
}

int solveGaussian(double **M, double *X, int N) {
    int i, j, p;
    double factor;

    // --- Forward Elimination ---
    for (p = 0; p < N - 1; p++) {
        if (M[p][p] == 0.0) {
            printf("\nError: Zero pivot element at M[%d][%d].\n", p, p);
            printf("The matrix may be singular. Pivoting is required.\n");
            return 1; 
        }

        for (i = p + 1; i < N; i++) {
            factor = M[i][p] / M[p][p]; 
            
            for (j = 0; j < N + 1; j++) {
                M[i][j] = M[i][j] - factor * M[p][j];
            }
        }
    }

    printf("\nUpper Triangular Matrix (from Gaussian):\n");
    printMatrix(M, N, N + 1);

    // --- Back Substitution ---
    for (i = N - 1; i >= 0; i--) {
        X[i] = M[i][N]; // Start with the constant term
        for (j = N - 1; j > i; j--) {
            X[i] -= M[i][j] * X[j]; // Subtract known values
        }
        if (M[i][i] == 0.0) {
             printf("\nError: Zero element at M[%d][%d] during back substitution.\n", i, i);
             printf("No unique solution exists.\n");
             return 1; 
        }
        X[i] = X[i] / M[i][i]; // Divide by the diagonal element
    }
    
    return 0; 
}

int solveLU(double **M, double *X, int N) {
    int i, j, p;
    double factor;
    double **L; // Lower triangular matrix
    double *Y;  // Intermediate solution vector for Ly = b
    double *b;  // Original 'b' vector (from M)

    // --- Allocate Memory for L, Y, and b ---
    Y = (double *)malloc(N * sizeof(double));
    b = (double *)malloc(N * sizeof(double));
    L = (double **)malloc(N * sizeof(double *));
    if (Y == NULL || b == NULL || L == NULL) {
        printf("Error: Memory allocation failed for LU components.\n");
        free(Y); free(b); free(L); // Free any that were successful
        return 1;
    }
    for (i = 0; i < N; i++) {
        L[i] = (double *)malloc(N * sizeof(double));
        if (L[i] == NULL) {
            printf("Error: Memory allocation failed for L row %d.\n", i);
            for (j = 0; j < i; j++) free(L[j]);
            free(L); free(Y); free(b);
            return 1;
        }
    }
    
    // Initialize L to zero and copy b from M
    for (i = 0; i < N; i++) {
        b[i] = M[i][N];
        for (j = 0; j < N; j++) {
            L[i][j] = 0.0;
        }
    }

    // M (NxN part) will become U.
    for (p = 0; p < N; p++) {
        
        L[p][p] = 1.0; // Diagonal of L is 1

        // Pivot check
        if (M[p][p] == 0.0) {
            printf("\nError: Zero pivot element at M[%d][%d] during LU decomposition.\n", p, p);
            printf("The matrix may be singular. Pivoting is required.\n");
            // Free memory before returning
            for(i=0; i<N; i++) free(L[i]);
            free(L); free(Y); free(b);
            return 1;
        }
        
        for (i = p + 1; i < N; i++) {
            factor = M[i][p] / M[p][p];
            L[i][p] = factor;
            // Note: j starts from p. Elements before p are already zero.
            // Note: j goes to N-1 (only modifying the 'A' part)
            for (j = p; j < N; j++) { 
                M[i][j] = M[i][j] - factor * M[p][j];
            }
        }
    }

    printf("\nLower Triangular Matrix (L):\n");
    printMatrix(L, N, N);
    printf("\nUpper Triangular Matrix (U):\n");
    printMatrix(M, N, N); // Print only the NxN part

    // --- 2. Forward Substitution (Solve Ly = b) ---
    for (i = 0; i < N; i++) {
        Y[i] = b[i]; // Start with the 'b' value
        for (j = 0; j < i; j++) {
            Y[i] -= L[i][j] * Y[j];
        }
    }

    // --- 3. Back Substitution (Solve Ux = y) ---
    // (Using M as U, and Y as the RHS)
    for (i = N - 1; i >= 0; i--) {
        X[i] = Y[i]; 
        for (j = i + 1; j < N; j++) { 
            X[i] -= M[i][j] * X[j]; 
        }
        if (M[i][i] == 0.0) {
             printf("\nError: Zero element at U[%d][%d] during back substitution.\n", i, i);
             printf("No unique solution exists.\n");
             // Free memory before returning
             for(j=0; j<N; j++) free(L[j]);
             free(L); free(Y); free(b);
             return 1; 
        }
        X[i] = X[i] / M[i][i]; 
    }
    
    // --- Free Memory ---
    for (i = 0; i < N; i++) {
        free(L[i]); // Free each row
    }
    free(L); // Free the array of pointers
    free(Y);
    free(b);

    return 0; 
}


int main(int argc, char *argv[]) {
    // create a 2d array called "M" of size N x N+1
    double **M;
    // create another matrix: X[0 .. N-1] to store the final solutions
    double *X;
    
    int i, j; 
    int N = 0; // Number of equations, to be read from argv
    int solveStatus = 1; // Default to failure
    
    if (argc != 4) {
        printf("Usage: %s <filename> <N> <method>\n", argv[0]);
        printf("  <filename>: Path to the matrix data file.\n");
        printf("  <N>: The number of equations (rows) in the system.\n");
        printf("  <method>: 'Gaussian' or 'LU'\n");
        exit(1);
    }

    N = atoi(argv[2]); 
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
        exit(1);
    }

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
    for (i = 0; i < N; i++) {
        M[i] = (double *)malloc((N + 1) * sizeof(double));
        if (M[i] == NULL) {
            printf("Error: Memory allocation failed for M (column %d).\n", i);
            // Free already allocated memory
            for (j = 0; j < i; j++) {
                free(M[j]);
            }
            free(M);
            free(X);
            fclose(file);
            exit(1);
        }
    }

    // --- File Reading (Now just one pass) ---
    printf("Reading augmented matrix from '%s'...\n", argv[1]);
    for (i = 0; i < N; i++) {
        for (j = 0; j < N + 1; j++) {
            if (fscanf(file, "%lf", &M[i][j]) != 1) {
                printf("Error reading data from file at row %d, col %d.\n", i, j);
                printf("Check if file has enough data for N=%d rows.\n", N);
                fclose(file);
                for (int k = 0; k < N; k++) free(M[k]);
                free(M);
                free(X);
                exit(1);
            }
        }
    }
    fclose(file); 

    printf("Initial Matrix:\n");
    printMatrix(M, N, N + 1);

    // --- Solve the System ---
    if (strcmp(argv[3], "Gaussian") == 0) {
        printf("\n--- Solving with Gaussian Elimination ---\n");
        solveStatus = solveGaussian(M, X, N);
    } else if (strcmp(argv[3], "LU") == 0) {
        printf("\n--- Solving with LU Decomposition ---\n");
        solveStatus = solveLU(M, X, N);
    } else {
        printf("Error: Unknown method '%s'. Please use 'Gaussian' or 'LU'.\n", argv[3]);
    }

    // --- Print Solution ---
    if (solveStatus == 0) {
        printf("\nSolution:\n");
        for (i = 0; i < N; i++) {
            printf("X[%d] = %f\n", i, X[i]);
        }
    } else {
        printf("\nCould not find a unique solution.\n");
    }

    // --- Free Dynamically Allocated Memory ---
    for (i = 0; i < N; i++) {
        free(M[i]);
    }
    free(M); 
    free(X);
    return 0;
}


