# Hamming Distance Search

This program creates a random `N`-bit block, overwrites a selected window with a supplied codeword, optionally flips the block according to a channel probability, then computes the Hamming distance between the codeword and candidate windows. Candidate positions are sorted by increasing distance, with position used to break ties.

## Build and run

```sh
gcc hamming_distance.c -o hamming_distance
./hamming_distance 128 12 0.1 101101
```

The arguments are total block length `N`, insertion position `d`, flip probability `p`, and a binary codeword. The code checks for positive sizes, a valid insertion range, and a probability from zero through one. Run the program without arguments to print its usage line.

## Files and interpretation

`hamming_distance_algorithm.txt` gives a step-by-step description of the approach. The output ranks candidate windows by distance; a smaller distance means a closer match to the supplied codeword. The current channel model flips all stored bytes with probability `p`, rather than flipping each bit independently.
