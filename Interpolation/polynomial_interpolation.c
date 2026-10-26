#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef struct {
    double x, y;
} Point;

double polynomial(double x, double coefficients[], int n);
double gaussian_error(double stdev);
void generate_points(Point points[], int m, double start, double step, double stdev, double coefficients[], int n);
double divided_difference(Point points[], int start_index, int end_index);
void lagrange_interpolation(Point points[], double lagrange_points[], int m);
void newton_interpolation(Point points[], double newton_points[], int m);
void calc_root_mean_square_error(Point points[], double lagrange_points[], double newton_points[], int m, int n_order);


int main(int argc, char *argv[]) {
    if (argc < 5) {
        fprintf(stderr, "Usage: %s <M> <N> <stdev> <a0> <a1> ... <aN>\n", argv[0]);
        fprintf(stderr, "  M: Number of points for interpolation (generates 2*M total points).\n");
        fprintf(stderr, "  N: Order of the polynomial.\n");
        fprintf(stderr, "  stdev: Standard deviation for Gaussian noise.\n");
        fprintf(stderr, "  a0...aN: N+1 coefficients for the polynomial.\n");
        return 1;
    }

    int M = atoi(argv[1]);
    int N = atoi(argv[2]);
    double stdev = atof(argv[3]);

    if (argc != 5 + N) {
        fprintf(stderr, "Error: Incorrect number of coefficients provided.\n");
        fprintf(stderr, "Expected %d coefficients for a polynomial of order %d, but got %d.\n", N + 1, N, argc - 4);
        return 1;
    }

    double *coefficients = (double *)malloc((N + 1) * sizeof(double));
    if (coefficients == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for coefficients.\n");
        return 1;
    }
    for (int i = 0; i <= N; i++) {
        coefficients[i] = atof(argv[i + 4]);
    }

    srand(time(NULL));

    double starting_value = (double)(rand() % 11) - 5.0; // Random double between -5 and 5
    double spacing = 2.0;

    Point *points = (Point *)malloc(2 * M * sizeof(Point));
     if (points == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for points array.\n");
        free(coefficients);
        return 1;
    }

    generate_points(points, M, starting_value, spacing, stdev, coefficients, N);

    double *lagrange_points = (double *)calloc(M, sizeof(double));
    double *newton_points = (double *)calloc(M, sizeof(double));

    if (lagrange_points == NULL || newton_points == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for interpolation result arrays.\n");
        free(coefficients);
        free(points);
        free(lagrange_points);
        free(newton_points);
        return 1;
    }


    lagrange_interpolation(points, lagrange_points, M);
    newton_interpolation(points, newton_points, M);

    // --- Error Calculation ---
    calc_root_mean_square_error(points, lagrange_points, newton_points, M, N);

    // --- Cleanup ---
    free(coefficients);
    free(points);
    free(lagrange_points);
    free(newton_points);

    return 0;
}

void generate_points(Point points[], int m, double start, double step, double stdev, double coefficients[], int n) {
    for (int i = 0; i < 2 * m; i++) {
        points[i].x = start + step * i;
        points[i].y = polynomial(points[i].x, coefficients, n) + gaussian_error(stdev);
    }
}

double polynomial(double x, double coefficients[], int n) {
    double ans = 0.0;
    for (int i = 0; i <= n; i++) {
        ans += coefficients[i] * pow(x, i);
    }
    return ans;
}

double gaussian_error(double stdev) {
    double u1 = ((double)rand() / (RAND_MAX)) + 1e-9;
    double u2 = ((double)rand() / (RAND_MAX)) + 1e-9;

    double z = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);

    return z * stdev;
}

void calc_root_mean_square_error(Point points[], double lagrange_points[], double newton_points[], int m, int n_order) {
    double lagrange_error = 0.0;
    double newton_error = 0.0;

    for (int i = 0; i < m; i++) {
        double original_y = points[2 * i + 1].y;
        lagrange_error += pow(original_y - lagrange_points[i], 2);
        newton_error += pow(original_y - newton_points[i], 2);
    }

    lagrange_error = sqrt(lagrange_error / m);
    newton_error = sqrt(newton_error / m);
    
    printf("N           lagrange_error          newton_error\n");
    printf("%d            %.6f                    %.6f\n", n_order, lagrange_error, newton_error);
}

void lagrange_interpolation(Point points[], double lagrange_points[], int m) {
    for (int i = 0; i < m; i++) {
        double interpolated_y = 0.0;
        double x_to_interpolate = points[2 * i + 1].x;

        for (int j = 0; j < m; j++) {
            double y_j = points[2 * j].y;
            double term = y_j;

            for (int k = 0; k < m; k++) {
                if (k != j) {
                    double x_k = points[2 * k].x;
                    double x_j = points[2 * j].x;
                    term *= (x_to_interpolate - x_k) / (x_j - x_k);
                }
            }
            interpolated_y += term;
        }
        lagrange_points[i] = interpolated_y;
    }
}

double divided_difference(Point points[], int start_index, int end_index) {
    if (start_index == end_index) {
        return points[2 * start_index].y;
    }

    return (divided_difference(points, start_index + 1, end_index) - divided_difference(points, start_index, end_index - 1)) /
           (points[2 * end_index].x - points[2 * start_index].x);
}

void newton_interpolation(Point points[], double newton_points[], int m) {
    for (int i = 0; i < m; i++) {
        double x_to_interpolate = points[2 * i + 1].x;
        double interpolated_y = 0;

        for (int j = 0; j < m; j++) {
            double coeff = divided_difference(points, 0, j);
            double term = coeff;

            for (int k = 0; k < j; k++) {
                term *= (x_to_interpolate - points[2 * k].x);
            }
            interpolated_y += term;
        }
        newton_points[i] = interpolated_y;
    }
}


