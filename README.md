# Numerical Methods Projects

A collection of small C programs exploring numerical algorithms, simulation, data analysis, and computational statistics. The projects are kept in separate folders so each can be compiled and used independently. Each folder includes its own README with the program interface and a sample command.

## Projects

| Project | What it covers |
| --- | --- |
| [Droplet Detection and Analysis](Droplet/README.md) | Smoothing voltage traces, detecting peaks and valleys, and measuring event width and spacing. |
| [Gaussian Elimination](Gauss/README.md) | Solving a linear system with forward elimination and back substitution. |
| [Hamming Distance Search](Hamming%20Distance/README.md) | Embedding a binary codeword in a random bit stream and ranking candidate matches under a simple channel model. |
| [Polynomial Interpolation](Interpolation/README.md) | Comparing Newton and Lagrange interpolation on noisy samples from a polynomial. |
| [2D Ising Model](Ising%20Model/README.md) | Sampling spin configurations with Metropolis updates and measuring magnetization over temperature. |
| [Logistic Map](Logistic%20map/README.md) | Iterating the logistic recurrence over a parameter range to produce bifurcation data. |
| [Linear System Solver](Matrix%20Inversion/README.md) | Comparing Gaussian elimination and LU decomposition for solving an augmented system. |
| [Two-Sample t-Test Simulation](ttest/README.md) | Generating normal samples and examining repeated two-sample t-statistics. |

## Requirements

The programs are written in C and can be built with a C compiler such as GCC. Some use functions from the math library and need `-lm` when linking. The logistic map and t-test programs can send plotting commands to [gnuplot](http://www.gnuplot.info/); their numerical output can still be inspected without a plotter where applicable.

## General workflow

Change into the folder for the project you want to use, compile its source file following that folder's README, and run the executable with the documented arguments. Output files are generally written to the current working directory. Several programs use random numbers, so separate runs may produce different results.

These are educational examples. The READMEs describe the interfaces implemented by the current source; they are not claims of production-grade numerical robustness.
