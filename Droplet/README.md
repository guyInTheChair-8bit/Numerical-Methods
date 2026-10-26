# Droplet Detection and Analysis

This program analyzes voltage traces stored in CSV files. It smooths the two voltage channels, searches for positive peaks in `Volt1` and negative valleys in `Volt2`, and reports measurements for detected events. You can pass one or more files, and the program processes them in sequence.

## Input

Each CSV data row is expected to contain three comma-separated numbers: time, `Volt1`, and `Volt2`. A header row is accepted. Lines beginning with `#` are skipped. The detection thresholds, smoothing window, and prominence settings are defined near the top of `droplet.c`.

## Build and run

```sh
gcc droplet.c -o droplet -lm
./droplet measurements.csv another_trace.csv
```

The output is printed in the terminal and includes event location, width at half height, and separation from the preceding event. `droplet_detection_pseudocode.txt` contains an earlier, simpler outline of peak detection; the current C program additionally handles two channels, smoothing, and threshold/prominence checks.

## Notes

The program expects numeric CSV columns in the order described above. It reports and skips a file if it cannot open it. Use traces that match the expected layout and units when interpreting the reported event measurements.
