// ==============================================================================
// Assignment:    Droplet Detection and Analysis
// Student RollNO: EE25B063 , EE25B110
// Date:          16 October 2025
// Assistance:    Gemini AI
// Description:   This C program analyzes  voltage data from CSV files
//                to detect  droplets. It takes  filename
//                as command-line argument and processes the file .
//
//                For each file, it performs positive peak detection on 'Volt1' and
//                negative valley detection on 'Volt2'. The process includes data
//                smoothing to reduce noise, dynamic thresholding based on signal
//                statistics, and prominence checks to identify significant events.
//
//                For each detected droplet, it calculates its location, width (FWHM),
//                and separation from the previous one.
// ==============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// --- These are the main settings we can change ---
#define SMOOTHING_WINDOW_SIZE 5      // How many points to average for smoothing. Must be an odd number.
#define HEIGHT_STD_DEV_FACTOR 3.0    // How many standard deviations away from the mean a point must be.
#define PROMINENCE_STD_DEV_FACTOR 1.5 // How much a peak/valley must stand out from the noise.
// ----------------------------------------------------

#define MAX_LINE_LENGTH 1024

// Data structures
typedef struct { double *t, *v1, *v2; int size, capacity; } Data;
typedef struct { double mean, std_dev; } Stats;

// This function reads the data from the CSV file.
Data read_csv(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Warning: Could not open file %s. Skipping.\n", filename);
        return (Data){NULL, NULL, NULL, 0, 0};
    }
    Data data = {NULL, NULL, NULL, 0, 10000};
    char line[MAX_LINE_LENGTH];
    data.t = malloc(data.capacity * sizeof(double));
    data.v1 = malloc(data.capacity * sizeof(double));
    data.v2 = malloc(data.capacity * sizeof(double));
    if (!data.t || !data.v1 || !data.v2) {
        perror("Initial memory allocation failed");
        exit(EXIT_FAILURE);
    }
    if (fgets(line, sizeof(line), file) && line[0] != '#' && (line[0] < '0' || line[0] > '9')) {}
    else { rewind(file); }
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#') continue;
        if (data.size >= data.capacity) {
            data.capacity *= 2;
            data.t = realloc(data.t, data.capacity * sizeof(double));
            data.v1 = realloc(data.v1, data.capacity * sizeof(double));
            data.v2 = realloc(data.v2, data.capacity * sizeof(double));
            if (!data.t || !data.v1 || !data.v2) {
                perror("Memory reallocation failed");
                exit(EXIT_FAILURE);
            }
        }
        if (sscanf(line, "%lf,%lf,%lf", &data.t[data.size], &data.v1[data.size], &data.v2[data.size]) == 3) {
            data.size++;
        }
    }
    fclose(file);
    return data;
}

// This function smooths the data by averaging points in a window.
double* smooth_data(const double* v_in, int size, int window_size) {
    if (window_size <= 1) return NULL;
    double* v_out = malloc(size * sizeof(double));
    if (!v_out) {
        perror("Failed to allocate memory for smoothed data");
        exit(EXIT_FAILURE);
    }
    int half_window = window_size / 2;
    for (int i = 0; i < size; ++i) {
        double sum = 0.0, count = 0;
        for (int j = i - half_window; j <= i + half_window; ++j) {
            if (j >= 0 && j < size) {
                sum += v_in[j];
                count++;
            }
        }
        v_out[i] = (count > 0) ? (sum / count) : v_in[i];
    }
    return v_out;
}

// This function calculates the mean and standard deviation.
Stats calculate_statistics(const double* v, int size) {
    Stats stats = {0.0, 0.0};
    if (size == 0) return stats;
    double sum = 0.0;
    for (int i = 0; i < size; ++i) sum += v[i];
    stats.mean = sum / size;
    double sum_sq_diff = 0.0;
    for (int i = 0; i < size; ++i) {
        sum_sq_diff += (v[i] - stats.mean) * (v[i] - stats.mean);
    }
    stats.std_dev = sqrt(sum_sq_diff / size);
    return stats;
}

// --- Specialized function to find POSITIVE PEAKS ---
void analyze_peaks(const char* col_name, const double* v, int size) {
    Stats stats = calculate_statistics(v, size);
    double height_threshold = stats.mean + (HEIGHT_STD_DEV_FACTOR * stats.std_dev);
    double prominence_threshold = PROMINENCE_STD_DEV_FACTOR * stats.std_dev;

    printf("\n--- Analysis for: %s (Detecting Positive Peaks) ---\n", col_name);
    printf("Statistics (on smoothed data): Mean=%.4f, StdDev=%.4f\n", stats.mean, stats.std_dev);
    printf("Dynamic Thresholds: Height > %.4f, Prominence > %.4f\n", height_threshold, prominence_threshold);
    printf("%-15s %-15s %-15s\n", "Location (idx)", "Width", "Separation");
    printf("--------------------------------------------------\n");

    int last_peak_location = 0, droplets_found = 0;
    for (int i = 1; i < size - 1; ++i) {
        if (v[i] > v[i-1] && v[i] > v[i+1] && v[i] > height_threshold) {
            double base_val1 = v[i], base_val2 = v[i];
            for (int j = i-1; j >= 0; --j) { if (v[j] < base_val1) base_val1 = v[j]; if (v[j] > v[i]) break; }
            for (int j = i+1; j < size; ++j) { if (v[j] < base_val2) base_val2 = v[j]; if (v[j] > v[i]) break; }
            double baseline = fmax(base_val1, base_val2);
            double prominence = v[i] - baseline;
            if (prominence > prominence_threshold) {
                droplets_found++;
                double half_max = baseline + prominence / 2.0;
                int left_idx = i, right_idx = i;
                for (int j = i; j > 0; --j) { if (v[j] <= half_max) break; left_idx = j; }
                for (int j = i; j < size; ++j) { if (v[j] <= half_max) break; right_idx = j; }
                printf("%-15d %-15d %-15d\n", i, right_idx - left_idx, (last_peak_location == 0) ? i : (i - last_peak_location));
                last_peak_location = i;
            }
        }
    }
    if (droplets_found == 0) printf("No droplets found in this column with the current settings.\n");
}

// --- Specialized function to find NEGATIVE VALLEYS ---
void analyze_valleys(const char* col_name, const double* v, int size) {
    Stats stats = calculate_statistics(v, size);
    double height_threshold = stats.mean - (HEIGHT_STD_DEV_FACTOR * stats.std_dev);
    double prominence_threshold = PROMINENCE_STD_DEV_FACTOR * stats.std_dev;

    printf("\n--- Analysis for: %s (Detecting Negative Valleys) ---\n", col_name);
    printf("Statistics (on smoothed data): Mean=%.4f, StdDev=%.4f\n", stats.mean, stats.std_dev);
    printf("Dynamic Thresholds: Height < %.4f, Prominence > %.4f\n", height_threshold, prominence_threshold);
    printf("%-15s %-15s %-15s\n", "Location (idx)", "Width", "Separation");
    printf("--------------------------------------------------\n");

    int last_peak_location = 0, droplets_found = 0;
    for (int i = 1; i < size - 1; ++i) {
        if (v[i] < v[i-1] && v[i] < v[i+1] && v[i] < height_threshold) {
            double base_val1 = v[i], base_val2 = v[i];
            for (int j = i-1; j >= 0; --j) { if (v[j] > base_val1) base_val1 = v[j]; if (v[j] < v[i]) break; }
            for (int j = i+1; j < size; ++j) { if (v[j] > base_val2) base_val2 = v[j]; if (v[j] < v[i]) break; }
            double baseline = fmin(base_val1, base_val2);
            double prominence = baseline - v[i];
            if (prominence > prominence_threshold) {
                droplets_found++;
                double half_max = baseline - prominence / 2.0;
                int left_idx = i, right_idx = i;
                for (int j = i; j > 0; --j) { if (v[j] >= half_max) break; left_idx = j; }
                for (int j = i; j < size; ++j) { if (v[j] >= half_max) break; right_idx = j; }
                printf("%-15d %-15d %-15d\n", i, right_idx - left_idx, (last_peak_location == 0) ? i : (i - last_peak_location));
                last_peak_location = i;
            }
        }
    }
    if (droplets_found == 0) printf("No droplets found in this column with the current settings.\n");
}

// The main function where the program starts.
int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "How to use: %s <file1.csv> [file2.csv] ...\n", argv[0]);
        return EXIT_FAILURE;
    }

    for (int i = 1; i < argc; ++i) {
        const char *filename = argv[i];
        printf("\n==================================================\n");
        printf("Processing file: %s\n", filename);
        printf("==================================================\n");

        Data data = read_csv(filename);
        if (data.size == 0) continue;

        double* smoothed_v1 = smooth_data(data.v1, data.size, SMOOTHING_WINDOW_SIZE);
        double* smoothed_v2 = smooth_data(data.v2, data.size, SMOOTHING_WINDOW_SIZE);

        // *** Use PEAK detection for Volt1 ***
        analyze_peaks("Volt1", smoothed_v1 ? smoothed_v1 : data.v1, data.size);

        // *** Use VALLEY detection for Volt2 ***
        analyze_valleys("Volt2", smoothed_v2 ? smoothed_v2 : data.v2, data.size);

        free(data.t); free(data.v1); free(data.v2);
        free(smoothed_v1); free(smoothed_v2);
    }
    return EXIT_SUCCESS;
}
