# Logistic Map

This program iterates the logistic recurrence `x[n+1] = r * x[n] * (1 - x[n])` over a range of control parameter values. It starts each parameter run at `x = 0.5`, discards the first 1000 iterations, and saves later values for a bifurcation-style plot.

## Build and run

```sh
gcc logistic_map.c -o logistic_map
./logistic_map 2.4 4.0 0.001 2000
```

The arguments are minimum `r`, maximum `r`, parameter step, and total iteration count. The step should be positive. The sample command scans a commonly examined range and retains iterations after the transient cutoff.

## Output and plotting

The program writes parameter, iteration, and state columns to `data.txt`, then invokes `gnuplot` to plot the parameter against the state. Install `gnuplot` and ensure it is on `PATH` for plotting. The plotting commands currently request a PNG rendering but use the output filename `logistic_map.jpg`; the checked-in image is named `logistic_map.png`.
