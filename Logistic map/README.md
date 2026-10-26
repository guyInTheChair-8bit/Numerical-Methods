# Logistic Map

Iterates the logistic equation over a range of parameter values and plots the resulting bifurcation data. Arguments are the minimum and maximum parameter, parameter step, and iteration count.

```sh
gcc logistic_map.c -o logistic_map
./logistic_map 2.4 4.0 0.001 2000
```

The plot routine invokes `gnuplot` and writes the generated data to `data.txt`.
