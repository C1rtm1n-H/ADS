#!/usr/bin/env python3
import csv
import matplotlib.pyplot as plt
import sys
import os

# python code to plot skip list performance results using the csv file
def main():
    csv_file = 'results/skip_list_data.csv'
    if not os.path.exists(csv_file):
        print(f"Error: {csv_file} not found.")
        print("Please run the performance test first.")
        sys.exit(1)

    data = {}

    try:
        with open(csv_file, 'r') as f:
            reader = csv.DictReader(f)
            for row in reader:
                op = row['Operation']
                n = int(row['N'])
                time = float(row['Time(ms)'])
                
                if op not in data:
                    data[op] = {'n': [], 'time': []}
                
                data[op]['n'].append(n)
                data[op]['time'].append(time)
    except Exception as e:
        print(f"Error reading CSV: {e}")
        sys.exit(1)

    
    for op in data:
        combined = sorted(zip(data[op]['n'], data[op]['time']))
        if combined:
            data[op]['n'], data[op]['time'] = zip(*combined)

    colors = {'insert': 'blue', 'find': 'green', 'delete': 'red'}
    
    
    fig, axes = plt.subplots(1, 2, figsize=(16, 6))
    
    # Plot 1: Total Time vs N
    ax1 = axes[0]
    for op, values in data.items():
        if not values['n']: continue
        color = colors.get(op, 'black')
        ax1.plot(values['n'], values['time'], 'o-', label=op, color=color, markersize=4)
    
    ax1.set_xlabel('N (Number of elements)')
    ax1.set_ylabel('Total Time (ms)')
    ax1.set_title('Total Time vs N\n(Expected: O(N log N))')
    ax1.grid(True, linestyle='--', alpha=0.7)
    ax1.legend()

    # Plot 2: Time per Operation (Time/N) vs N
    ax2 = axes[1]
    for op, values in data.items():
        if not values['n']: continue
        n_vals = values['n']
        time_vals = values['time']
        
        time_per_op = [t / n for t, n in zip(time_vals, n_vals)]
        
        color = colors.get(op, 'black')
        ax2.plot(n_vals, time_per_op, 'o-', label=op, color=color, markersize=4)

    ax2.set_xlabel('N (Number of elements)')
    ax2.set_ylabel('Time per Operation (ms)')
    ax2.set_title('Average Time per Operation (Time / N) vs N\n(Expected: O(log N))')
    ax2.grid(True, linestyle='--', alpha=0.7)
    ax2.legend()

    plt.tight_layout()
    output_file = 'results/skip_list_performance.png'
    plt.savefig(output_file)
    print(f"Plot saved to {output_file}")

if __name__ == "__main__":
    main()
