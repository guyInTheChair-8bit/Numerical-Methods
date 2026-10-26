/*
 * =====================================================================================
 *
 * Filename:  EE25B110_ising.c
 *
 * Description:  A C program to simulate the 2D Ising model using the
 * Metropolis Monte Carlo algorithm. It calculates the average
 * magnetization of a spin lattice over a range of temperatures
 * and saves the output to "data.txt".
 *
 * Authors:  EE25B110, EE25B063, EE25B024
 *
 * Last Modified:  17 September 2025
 *
 * To Compile:  gcc -o ising_model EE25B110.c -lm
 *
 * To Run:  ./ising_model N J Tstart Tstop dT P dMav
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int N;//Dimension Of the lattice
double J;//Coupling Co-efficient

//Function Signatures
void randomize(int *lattice);
double avg(const int *lattice);
void metropolis(int *old_lattice, int *new_lattice, double temperature);
int pick_random_site();
double calc_energy_diff(int spin_index, const int *lattice);

int main(int argc, char **argv) {  //Passing arguments through the command line.

    if (argc != 8) {
        fprintf(stderr, "Usage: %s N J Tstart Tstop dT P dMav\n", argv[0]);
        return 1;
    }

    N = atoi(argv[1]);
    J = atof(argv[2]);
    double Tstart = (double)atof(argv[3]);
    double Tstop = (double)atof(argv[4]);
    double dT = (double)atof(argv[5]);
    int P = atoi(argv[6]);
    double dMav = (double)atof(argv[7]);

    int num_sites = N * N;
    //Allocating sufficient memory for the Arrays Mold, Mnew.
    int *Mold = (int *)malloc(num_sites * sizeof(int));
    int *Mnew = (int *)malloc(num_sites * sizeof(int));

    if (Mold == NULL || Mnew == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for lattices.\n");
        free(Mold);
        free(Mnew);
        return 1;
    }

    srand(time(NULL));
//File Output: It writes the Temperature and Magnetization for each step into data.txt.
    FILE *output_file = fopen("data.txt", "w");
    if (output_file == NULL) {
        fprintf(stderr, "Error: Could not open data.txt for writing.\n");
        free(Mold);
        free(Mnew);
        return 1;
    }
    
    printf("Lattice Size: %dx%d, J: %.2f, Temp Range: %.2f-%.2f, dT: %.2f\n", N, N, J, Tstart, Tstop, dT);
    
    int p;
    double final_magnetization;

    randomize(Mold);  // Initialize the lattice with a random configuration of spins.

    // This loop runs until the system reaches thermal equilibrium for the current temperature T.
    for (double T = Tstart; T <= Tstop; T += dT) {
        
        p = 1; 
        while (1) {
            
            if (p > 1 && (p >= P || fabs(avg(Mnew) - avg(Mold)) < dMav)) {
                break;
            }

            if (p%2 == 1)
              metropolis(Mold, Mnew, T);
            else
              metropolis(Mnew, Mold, T);
            // Swap pointers to make the new lattice the old one for the next iteration.
            // This is much more efficient than copying the entire lattice array.
            
            int *temp = Mold;
            Mold = Mnew;
            Mnew = temp;
            
            p++;
        }

        double Msum = 0.0;
        int no_of_averaging_steps = 2000;
        for (int i = 0; i < no_of_averaging_steps; i++) {
            metropolis(Mold, Mnew, T);

            int *temp = Mold;
            Mold = Mnew;
            Mnew = temp;
            
            Msum += (i%2 == 0) ? avg(Mold) : avg(Mnew);
        }


        final_magnetization = Msum/(double)no_of_averaging_steps;
        fprintf(output_file, "%f\t%f\n", T, final_magnetization);
        printf("Temperature: %5.2f, Avg. Magnetization: %8.5f, Iterations: %d\n", T, final_magnetization, p);
    }

    fclose(output_file);
    free(Mold);
    free(Mnew);

    printf("Simulation finished. Results saved to data.txt.\n");

    return 0;
}
//Function to fill the lattice with 1's and -1's as elements at random.
void randomize(int *lattice) {
    for (int i = 0; i < N * N; i++) {
        lattice[i] = (rand() % 2) * 2 - 1;
    }
}
//Function to calculate the sum of spins of a lattice.
double avg(const int *lattice) {
    int sum = 0;
    for (int i = 0; i < N * N; i++) {
        sum += lattice[i];
    }
    return fabs((double)sum / (N * N));
}

//Function for implementing metropolis algorithm.
void metropolis(int *old_lattice, int *new_lattice, double temperature) {
    for (int i = 0; i < N * N; i++) {
        new_lattice[i] = old_lattice[i];
    }

    for (int i = 0; i < N * N; i++) {
        int spin_index = pick_random_site();
        double dE = calc_energy_diff(spin_index, new_lattice);

        if (dE <= 0)
            new_lattice[spin_index] *= -1;
        else if(((double)rand() / RAND_MAX) < exp(-dE / temperature)) {
            new_lattice[spin_index] *= -1; 
        }
    }
}
//Function to pick a random element.
int pick_random_site() {
    return rand() % (N * N);
}
//This function calculates the change in energy (ΔE) that would result from flipping a single "spin" at a specific location (spin_index) within a 2D lattice.
double calc_energy_diff(int spin_index, const int *lattice) {
    int row = spin_index / N; // Get 2D coordinates from 1D index.
    int col = spin_index % N;

    int right = (row * N) + (col + 1) % N;
    int left = (row * N) + (col - 1 + N) % N;
    int up = (((row - 1 + N) % N) * N) + col;
    int down = (((row + 1) % N) * N) + col;

    int sum_of_neighbors = lattice[right] + lattice[left] + lattice[up] + lattice[down];

    return 2 * J * sum_of_neighbors * lattice[spin_index];
}

