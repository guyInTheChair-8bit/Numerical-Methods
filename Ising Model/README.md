# 2D Ising Model

The program simulates an `N` by `N` lattice of spins using Metropolis Monte Carlo updates. It sweeps through a temperature range, allowing the lattice to relax at each temperature, and records the average magnetization. The main implementation is `ising_model.c`; `ising_model_test.c` is a separate test/variant source file.

## Build and run

```sh
gcc ising_model.c -o ising_model -lm
./ising_model 20 1 1.5 4.0 0.1 1000 0.001
```

Arguments are lattice width `N`, coupling constant `J`, starting temperature, ending temperature, temperature increment, maximum relaxation iterations `P`, and the magnetization-change tolerance `dMav`. Choose a positive temperature step and a range whose end is at least the start.

## Output

The program prints the lattice parameters and a summary for each temperature, including average magnetization and relaxation iterations. It writes temperature and magnetization pairs to `data.txt` in the current working directory, replacing that file if it already exists.

## Visuals

- `magnetization_vs_temperature_coupling_comparison.jpeg` plots average magnetization against temperature for several coupling values.
- `spin_lattice_simulation.mp4` shows a visualization of the spin lattice evolving during a simulation.

## Notes

The initial spin configuration and update choices are random, so results can differ between runs. The program does not accept a random seed as an argument.
