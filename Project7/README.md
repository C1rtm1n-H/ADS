# ADS Project 7: Skip List

This project implements a Skip List data structure in C and analyzes its performance (Insertion, Search, Deletion) across different input sizes. It demonstrates the $O(\log N)$ expected time complexity of Skip List operations.

## Prerequisites

- **GCC Compiler**: For compiling the C code.
- **Python 3**: For running the plotting script.
- **Matplotlib**: Python library for generating performance graphs.

To install Matplotlib:
```bash
pip install matplotlib
```

## Directory Structure

- `src/`: Source code (`main.c`, `skip_list.c`, `skip_list.h`)
- `bin/`: Compiled executables
- `results/`: Generated CSV data and performance plots
- `report/`: LaTeX report files
- `Makefile`: Build automation
- `plot.py`: Python script for visualization

## Usage

### 1. Build the Project

Compile the source code to generate the executable.

```bash
make
```

### 2. Run Performance Tests

Run the benchmark tests. This will execute insertion, search, and deletion operations for input sizes ranging from 1,000 to 1,000,000.
The results will be saved to `results/skip_list_data.csv`.

```bash
make run
```

### 3. Generate Plots

Use the Python script to visualize the performance data.
This will generate `results/skip_list_performance.png`.

```bash
make plot
```

### 4. Clean Up

Remove compiled binaries and generated results.

```bash
make clean
```
