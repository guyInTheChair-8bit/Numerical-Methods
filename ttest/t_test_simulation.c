//ee25b110, ee25b078, ee25b082

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void box_muller(double* arr1, double* arr2, double* new_arr1, double* new_arr2, int n);
void scale_distribution(double* arr, double stdev, double mean, int n);
double calc_mean(double* arr, int n);
double calc_stdev(double* arr, double mean, int n);
double t_test(double* arr1, double* arr2, int n);
void plot_t_values(double* t_values, int start_n, int num_experiments);

int main(int argc, char *argv[]) {
    // Check for the correct number of command-line arguments.
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <mean1> <mean2> <stdev> <N>\n", argv[0]);
        return 1; 
    }

    double mean1 = atof(argv[1]);
    double mean2 = atof(argv[2]);
    double stdev = atof(argv[3]);
    int N = atoi(argv[4]);

    if (N <= 1) {
        fprintf(stderr, "Error: N must be greater than 1.\n");
        return 1;
    }

    srand((unsigned int)time(NULL));

    // Allocate memory for the two arrays of uniform random numbers.
    double* arr1 = (double*)malloc(N * sizeof(double));
    double* arr2 = (double*)malloc(N * sizeof(double));

    // Allocate memory for the arrays that will hold the normally distributed numbers.
    double* new_arr1 = (double*)malloc(N * sizeof(double));
    double* new_arr2 = (double*)malloc(N * sizeof(double));

    // Check if memory allocation was successful.
    if (arr1 == NULL || arr2 == NULL || new_arr1 == NULL || new_arr2 == NULL) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        // Free any memory that was successfully allocated before exiting.
        free(arr1);
        free(arr2);
        free(new_arr1);
        free(new_arr2);
        return 1;
    }

    int num_experiments = 30;
    double* t_values = (double*)malloc(num_experiments * sizeof(double));
    if (t_values == NULL) {
        fprintf(stderr, "Error: Memory allocation for t_values failed.\n");
        free(arr1);
        free(arr2);
        free(new_arr1);
        free(new_arr2);
        return 1;
    }

    printf("Running %d experiments...\n", num_experiments);
    printf("Initial N = %d, Mean1 = %.2f, Mean2 = %.2f, Stdev = %.2f\n\n", N, mean1, mean2, stdev);
    printf("%-10s | %-10s\n", "Experiment", "t-value");
    printf("-----------|-----------\n");

    for (int j = 0; j < num_experiments; j++) {

        // Generate two arrays of uniformly distributed random numbers between 0.0 and 1.0.
        for (int i = 0; i < N; i++) {
            arr1[i] = (double)rand() / (RAND_MAX + 1.0);
            arr2[i] = (double)rand() / (RAND_MAX + 1.0);
        }

        box_muller(arr1, arr2, new_arr1, new_arr2, N);
        scale_distribution(new_arr1, stdev, mean1, N);
        scale_distribution(new_arr2, stdev, mean2, N);
        t_values[j] = t_test(new_arr1, new_arr2, N);
        
        printf("%-10d | %-10.4f\n", j + 1, t_values[j]);
    }

    plot_t_values(t_values, N, num_experiments);

    // Free all dynamically allocated memory to prevent memory leaks.
    free(arr1);
    free(arr2);
    free(new_arr1);
    free(new_arr2);
    free(t_values);

    return 0; 
}



/**
 * @brief Applies the Box-Muller transform to two arrays of uniformly
 * distributed random numbers to generate two arrays of normally
 * distributed random numbers (with mean 0 and stdev 1).
 */
void box_muller(double* arr1, double* arr2, double* new_arr1, double* new_arr2, int n) {
    for (int i = 0; i < n; i++) {
        if (arr1[i] == 0) arr1[i] = 1e-9;
        double r = sqrt(-2.0 * log(arr1[i]));
        double theta = 2.0 * M_PI * arr2[i];
        new_arr1[i] = r * cos(theta);
        new_arr2[i] = r * sin(theta);
    }
}

/**
 * @brief Scales and shifts a normally distributed array to a new mean and standard deviation.
 */
void scale_distribution(double* arr, double stdev, double mean, int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = arr[i] * stdev + mean;
    }
}

/**
 * @brief Calculates the arithmetic mean of an array of doubles.
 */
double calc_mean(double* arr, int n) {
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        sum += arr[i];
    }
    return sum / n;
}

/**
 * @brief Calculates the sample standard deviation of an array of doubles.
 */
double calc_stdev(double* arr, double mean, int n) {
    double sum_sq_diff = 0.0;
    for (int i = 0; i < n; i++) {
        sum_sq_diff += pow(arr[i] - mean, 2);
    }
    return sqrt(sum_sq_diff / (n - 1));
}

/**
 * @brief Performs an independent two-sample t-test assuming equal variances.
 */
double t_test(double* arr1, double* arr2, int n) {
    double x1 = calc_mean(arr1, n);
    double x2 = calc_mean(arr2, n);
    double sx1 = calc_stdev(arr1, x1, n);
    double sx2 = calc_stdev(arr2, x2, n);
    double sp = sqrt((pow(sx1, 2) + pow(sx2, 2)) / 2.0);
    if (sp == 0) return 0.0;
    return (x1 - x2) / (sp * sqrt(2.0 / n));
}

// @brief Writes the calculated t-values to a text file and attempts to plot them using Gnuplot.
void plot_t_values(double* t_values, int start_n, int num_experiments) {
    const char* data_filename = "t_values.txt";
    FILE *file = fopen(data_filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Could not open file for writing results.\n");
        return;
    }

    // Write data file with Experiment number and t-value
    fprintf(file, "# Experiment\tt_value\n");
    for (int i = 0; i < num_experiments; i++) {
        fprintf(file, "%d\t%.4f\n", start_n + i * 100, t_values[i]);
    }
    fclose(file);
    printf("\nResults have been saved to %s.\n", data_filename);

    // --- Gnuplot Integration ---
    // Use popen to pipe commands to Gnuplot
    FILE *gnuplot_pipe = popen("gnuplot -persistent", "w");
    if (gnuplot_pipe) {
        printf("Attempting to generate plot using Gnuplot...\n");
        fprintf(gnuplot_pipe, "set title 'T-Test Results (N=%d)'\n", start_n);
        fprintf(gnuplot_pipe, "set xlabel 'N'\n");
        fprintf(gnuplot_pipe, "set ylabel 'T-Value'\n");
        fprintf(gnuplot_pipe, "set grid\n");
        fprintf(gnuplot_pipe, "plot '%s' using 1:2 with linespoints title 'T-Values'\n", data_filename);
        fflush(gnuplot_pipe); // Flush the pipe to ensure commands are sent
        pclose(gnuplot_pipe);
        printf("Plot window should have opened. If not, ensure Gnuplot is installed and in your system's PATH.\n");
    } else {
        printf("\nCould not open pipe to Gnuplot. Please ensure Gnuplot is installed.\n");
    }

}
