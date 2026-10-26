# Two-Sample t-Test Simulation

Generates normally distributed samples with the Box–Muller transform, performs repeated two-sample t-tests, and plots the resulting t-statistics. The program accepts the two population means, shared standard deviation, and sample size.

```sh
gcc t_test_simulation.c -o t_test_simulation -lm
./t_test_simulation 0 1 1 100
```

The program uses `gnuplot` to create a plot; install it and make it available on `PATH` for plotting.
