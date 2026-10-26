# Two-Sample t-Test Simulation

This program draws two samples of size `N`, transforms uniform random values into normally distributed values using the Box–Muller method, shifts them to the requested means, and computes a two-sample t-statistic. It repeats this experiment 30 times and displays each statistic.

## Build and run

```sh
gcc t_test_simulation.c -o t_test_simulation -lm
./t_test_simulation 0 1 1 100
```

Arguments are the first sample mean, second sample mean, shared standard deviation, and sample size `N` (which must be greater than one). The program seeds its random number generator from the current time, so results vary between runs.

## Results and plotting

The program prints the t-statistic for each experiment and writes the plotted data to `t_values.txt` in the current working directory. It then attempts to open a Gnuplot window. Install `gnuplot` and make it available on `PATH` for the plot; the numerical results are printed even if Gnuplot is unavailable. `t_test_results.jpg` is a saved plot image included with the project.
