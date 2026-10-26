# Droplet Detection and Analysis

Analyzes time-series CSV input to locate positive peaks in `Volt1` and negative valleys in `Volt2`. It smooths signals and reports event location, width, and separation. `droplet_detection_pseudocode.txt` records an earlier outline of the detection workflow.

```sh
gcc droplet.c -o droplet -lm
./droplet measurements.csv
```

The input is expected to contain time and two voltage columns.
