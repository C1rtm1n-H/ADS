import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
import os

# Define directories
results_dir = 'results'
csv_file = os.path.join(results_dir, 'result.csv')

# Ensure the results directory exists for saving plots
os.makedirs(results_dir, exist_ok=True)

try:
    # Read the csv file
    df = pd.read_csv(csv_file, header=0)
    sns.set_theme(style="whitegrid")

    # Define colors and markers for consistency
    palette = {
        'Algorithm1_OptimizedDP': 'green',
        'Algorithm2_DirectDP': 'blue',
        'Algorithm3_ComplementaryDP': 'orange',
        'Algorithm4_BruteForce': 'red'
    }
    markers = {
        'Algorithm1_OptimizedDP': 'o',
        'Algorithm2_DirectDP': 's',
        'Algorithm3_ComplementaryDP': '^',
        'Algorithm4_BruteForce': 'X'
    }

    # --- Plot 1: Linear Scale (for context) ---
    plt.figure(figsize=(12, 8))
    for name, group in df.groupby('TestAlgorithm'):
        # Only plot algorithms that are meaningful on a linear scale with large N
        if name in ['Algorithm1_OptimizedDP', 'Algorithm2_DirectDP']:
             sns.lineplot(data=group, x='N', y='Runtime(ms)', marker=markers.get(name), color=palette.get(name), label=name)
    plt.title('Performance on Linear Scale', fontsize=16)
    plt.xlabel('Input Size (N)', fontsize=12)
    plt.ylabel('Runtime (ms)', fontsize=12)
    plt.legend()
    plt.tight_layout()
    plt.savefig(os.path.join(results_dir, 'performance_linear_scale.png'), dpi=300)
    print("Generated linear scale plot.")

    # --- Plot 2: Log-Log Scale (for comprehensive comparison) ---
    plt.figure(figsize=(12, 8))
    for name, group in df.groupby('TestAlgorithm'):
        sns.lineplot(data=group, x='N', y='Runtime(ms)', marker=markers.get(name), color=palette.get(name), label=name)
    
    plt.xscale('log')
    plt.yscale('log')
    plt.title('Performance Comparison on Log-Log Scale', fontsize=16)
    plt.xlabel('Input Size (N)', fontsize=12)
    plt.ylabel('Runtime (ms)', fontsize=12)
    plt.grid(True, which="both", ls="--")
    plt.legend()
    plt.tight_layout()
    plt.savefig(os.path.join(results_dir, 'performance_log_log_scale.png'), dpi=300)
    print("Generated log-log scale plot.")


except FileNotFoundError:
    print(f"Error: File not found at '{csv_file}'.")
    print("Please make sure 'result.csv' is in the same directory as your Python script.")
except Exception as e:
    print(f"An error occurred while processing the file: {e}")