# Compiler and interpreter
CC = gcc
PYTHON = python3

# Directories
SRCDIR = src
OBJDIR = obj
BINDIR = bin
RESULTSDIR = results

# Compiler flags
CFLAGS = -std=c99 -Wall -O2 -g -mcmodel=large

# Target executable
TARGET = $(BINDIR)/project_6

# Source files
SOURCES = $(wildcard $(SRCDIR)/*.c)
# Generate corresponding object files in OBJDIR
OBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))

# CSV result file path
RESULT_CSV = $(RESULTSDIR)/result.csv

# Primary Targets

# Default target: build the executable
all: $(TARGET)

# Target to run the full workflow: build, execute, and plot
run: $(TARGET)
    @echo "Running executable..."
    ./$(TARGET)
    @mkdir -p $(RESULTSDIR)
    @echo "Moving results to $(RESULTSDIR)/"
    @mv result.csv $(RESULT_CSV)
    -@mv visual_*.csv $(RESULTSDIR)/ 2>/dev/null || true
    @$(MAKE) plot
    @$(MAKE) plot_ratio
    @$(MAKE) visual

# Link the final executable
$(TARGET): $(OBJECTS)
    @mkdir -p $(BINDIR)
    $(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET) -lm
    @echo "Successfully built $(TARGET)"

# Compile source files to object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
    @mkdir -p $(OBJDIR)
    $(CC) $(CFLAGS) -c $< -o $@

# Target to generate plots from the CSV file
plot:
    @echo "Generating plots from $(RESULT_CSV)..."
    $(PYTHON) plot.py
    @echo "Plots generated in $(RESULTSDIR)/"

plot_ratio:
    @echo "Generating ratio plots..."
    $(PYTHON) plot_ratio.py

# Target to visualize packing results
visual:
    @echo "Generating visualizations..."
    $(PYTHON) visualize.py $(RESULTSDIR)/visual_*.csv

# Clean up build artifacts and results
clean:
    @echo "Cleaning up project..."
    rm -rf $(OBJDIR) $(BINDIR)
    # Optional: clean results too if you want
    # rm -rf $(RESULTSDIR)
    @echo "Cleanup complete."

.PHONY: all run plot visual plot_ratio clean