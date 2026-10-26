# Polynomial Interpolation

Generates noisy samples from a polynomial, evaluates Lagrange and Newton interpolation, and reports root-mean-square errors. Arguments provide the point count, polynomial order, noise standard deviation, and polynomial coefficients.

```sh
gcc polynomial_interpolation.c -o polynomial_interpolation -lm
./polynomial_interpolation 5 2 0.1 1 2 1
```
