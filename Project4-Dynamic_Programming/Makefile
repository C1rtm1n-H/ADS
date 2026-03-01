# Compiler and interpreter
CC = gcc
PYTHON = python3

# Directories
SRCDIR = src
OBJDIR = obj
BINDIR = bin
RESULTSDIR = results

# Compiler flags
# -Wall: Enable all warnings
# -O2: Optimization level 2
# -g: Add debugging information
CFLAGS = -std=c99 -Wall -O2 -g -mcmodel=large

# Target executeble
TARGET = $(BINDIR)/project_4

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
run: $(TARGET) $(RESULT_CSV) plot

# Link the final executable
$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET) -lm # lm for math library
	@echo "Successfully built $(TARGET)"

# Compile source files to object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled $< -> $@"

# Target to generate results CSV by running the executable
$(RESULT_CSV): $(TARGET)
	@mkdir -p $(RESULTSDIR)
	@echo "Running executable to generate $(RESULT_CSV)..."
	./$(TARGET)
	@echo "Moving result.csv to $(RESULTSDIR)/"
	@mv result.csv $(RESULT_CSV)

# Target to generate plots from the CSV file
plot: $(RESULT_CSV)
	@echo "Generating plots from $(RESULT_CSV)..."
	$(PYTHON) plot.py
	@echo "Plots generated in $(RESULTSDIR)/"

# Clean up build artifacts and results
clean:
	@echo "Cleaning up project..."
	rm -rf $(OBJDIR) $(BINDIR) $(RESULTSDIR)
	@echo "Cleanup complete."

# Phony targets are not actual files
.PHONY: all run plot clean
