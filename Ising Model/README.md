# 2D Ising Model

Simulates a two-dimensional spin lattice with the Metropolis Monte Carlo algorithm and records average magnetization over a temperature range. `ising_model_test.c` contains a separate test/variant implementation.

```sh
gcc ising_model.c -o ising_model -lm
./ising_model 20 1 1.5 4.0 0.1 1000 0.001
```

Results are written to `data.txt` in the current directory.
