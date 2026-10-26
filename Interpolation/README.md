# Polynomial Interpolation

This program constructs a polynomial from command-line coefficients, generates `2*M` evenly spaced sample points with Gaussian noise, and compares Newton and Lagrange interpolation. It uses one set of points to construct the interpolants and the remaining points to estimate root-mean-square interpolation error.

## Build and run

```sh
gcc polynomial_interpolation.c -o polynomial_interpolation -lm
./polynomial_interpolation 5 2 0.1 1 2 1
```

Arguments are `M` (number of points per set), polynomial order `N`, noise standard deviation, followed by exactly `N + 1` coefficients `a0` through `aN`. Coefficients are interpreted in ascending powers, so the example represents `1 + 2x + x^2`. The program reports the two interpolation errors in the terminal.

## Files and notes

`interpolation_algorithm.txt` contains a pseudocode outline of the approach. The program chooses a random starting x-coordinate and generates random Gaussian noise, so the input points and reported errors can vary between runs. A standard deviation of zero disables the added noise.
