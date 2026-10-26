#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Structure to store a single result (location and its Hamming distance)
typedef struct {
    int location;
    int distance;
} Result;

// --- Helper Functions ---


//  Get the value of a single bit (0 or 1) from the block at a specific bit_position.
int get_bit(unsigned char *block, int bit_position) {
    int byte_index = bit_position / 8;
    int bit_index_in_byte = bit_position % 8;

    // Shifts the desired bit to the rightmost position and ANDs with 1 to isolate it.
    return (block[byte_index] >> bit_index_in_byte) & 1;
}

//Comparison function for qsort to sort results by distance.

int compare_results(const void *a, const void *b) {
    // Sort primarily by distance (ascending)
    int diff = ((Result*)a)->distance - ((Result*)b)->distance;
    if (diff != 0) {
        return diff;
    }
    // Secondary sort by location (d) if distances are equal
    return ((Result*)a)->location - ((Result*)b)->location;
}

// --- Essential Functions ---

//Generates an N-bit block of uniform random binary bits.
unsigned char *create_block(int N) {
    int required_no_of_bytes = (N + 7) / 8;
    // use calloc to assign the required number of bytes and initialize all to zero
    unsigned char *block = (unsigned char *)calloc(required_no_of_bytes, sizeof(unsigned char));
    if (block == NULL) {
        perror("Failed to allocate memory for block");
        exit(EXIT_FAILURE);
    }

    // Populate with random bits
    for (int i = 0; i < N; i++) {
        // r = rand() % 2
        if (rand() % 2 == 1) {
            int byte_index = i / 8;
            int bit_index = i % 8;

            unsigned char bit_mask = 1 << bit_index;
            // block[byte_index] = block[byte_index] | bit
            block[byte_index] = block[byte_index] | bit_mask;
        }
    }
    return block;
}

// Replaces M bits in the block starting at location d with the codeword.
void replace_with_codeword(unsigned char *block, int d, int M, const char *codeword) {
    for (int i = 0; i < M; i++) {
        int target_index = d + i;

        int byte_index = target_index / 8;
        int bit_index = target_index % 8;

        unsigned char mask = 1 << bit_index;

        if (codeword[i] == '1') {
            // block[byte_index] = block[byte_index] | mask
            block[byte_index] = block[byte_index] | mask;
        } else if (codeword[i] == '0') {
            // block[byte_index] = block[byte_index] & (~mask)
            block[byte_index] = block[byte_index] & (~mask);
        }
    }
}

//Implements flip_based_on_prob: Flips ALL N bits with probability 'p'.
void flip_based_on_prob(double p, unsigned char *block, int N) {
    // Generate a random float between 0.0 and 1.0
    double r = (double)rand() / (double)RAND_MAX;
    int no_of_bytes = (N + 7) / 8;

    // Logic: Flips all bits with probability p. If r > p, we return (do nothing).
    if (r > p) {
        return; // Probability (1-p) event
    } else {
        // Probability p event
        for (int i = 0; i < no_of_bytes; i++) {
            block[i] = ~block[i]; // block[i] = ~block[i]
        }
        printf("\n[CHANNEL]: All N bits were flipped due to probability p=%.2f.\n", p);
    }
}

//Implements create_hamming_list: Searches the block for the codeword and calculates Hamming distances.
Result *create_hamming_list(unsigned char *block, const char *codeword_string, int N, int M) {
    int max_locations = N - M + 1;
    Result *list_A = (Result *)malloc(max_locations * sizeof(Result));
    if (list_A == NULL) {
        perror("Failed to allocate memory for results list");
        exit(EXIT_FAILURE);
    }

    // Outer loop: i is the starting position of the comparison in the block
    for (int i = 0; i < max_locations; i++) {
        int ham_dist = 0; // Reseting distance for the new starting location for recording it later.

        // j iterates over the M bits of the codeword
        for (int j = 0; j < M; j++) {
            // Calculate the exact position of the bit in the N-bit block
            int global_bit_position = i + j;

            // Extract the bit from the block using the function (block_bit = ...)
            int block_bit = get_bit(block, global_bit_position);

            // Get the bit from the codeword (char to int conversion)
            int codeword_bit = (codeword_string[j] == '1') ? 1 : 0;

            if ((codeword_bit ^ block_bit) == 1) {//Exclusive OR operator gives us 1 if we have dont have matching values.
                ham_dist++;
            }
        }

        // push (i, ham_dist) as a struct into a list A
        list_A[i].location = i;
        list_A[i].distance = ham_dist;
    }

    // arrange the elements in the list in increasing order of hamming distance
    qsort(list_A, max_locations, sizeof(Result), compare_results);

    return list_A;
}

// --- Main Program ---

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <N:total_bits> <d:start_location> <p:flip_prob> <codeword:M_bits>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    int d = atoi(argv[2]);
    double p = atof(argv[3]);
    const char *codeword = argv[4];
    int M = strlen(codeword);

    if (N <= 0 || M <= 0 || d < 0 || d >= (N - M) || p < 0.0 || p > 1.0) {
        fprintf(stderr, "\nInvalid inputs. Constraints:\nN > 0, M > 0, 0 <= d < (N - M), 0.0 <= p <= 1.0.\n");
        return 1;
    }

    // srand(NULL)
    srand((unsigned int)time(NULL));

    printf("--- Codeword Search Simulation ---\n");
    printf("N (Total Block Size): %d\n", N);
    printf("M (Codeword Size): %d\n", M);
    printf("d (True Location): %d\n", d);
    printf("p (Flip Probability): %.2f\n", p);
    printf("Codeword: %s\n", codeword);


    unsigned char *block = create_block(N);

    replace_with_codeword(block, d, M, codeword);

    flip_based_on_prob(p, block, N);

    int max_locations = N - M + 1;
    Result *hamming_list = create_hamming_list(block, codeword, N, M);

    // print d and H from hamming_list in two columns with serial no's on the sides
    printf("\n--- Top %d Results (Location and Hamming Distance) ---\n", max_locations < 10 ? max_locations : 10);
    printf("No. | Location (d) | Hamming Dist (H)\n");

    int output_count = (max_locations < 10) ? max_locations : 10;
    for (int i = 0; i < output_count; i++) {
        printf("%-3d | %-12d | %-8d", i + 1, hamming_list[i].location, hamming_list[i].distance);
        if (hamming_list[i].location == d) {
            printf("  <-- True Location");
        }
        printf("\n");
    }

    // Cleanup
    free(block);
    free(hamming_list);

    return 0;
}
