
import pandas as pd
import matplotlib.pyplot as plt

def main():
    # read the csv file
    df = pd.read_csv('results/result.csv')
    
    test_modes = df['TestMode'].unique()
    algorithms = df['Algorithm'].unique()
    
    colors = ['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728', '#9467bd']
    markers = ['o', 's', '^', 'D', 'v']
    
    fig, axes = plt.subplots(1, 3, figsize=(18, 6))
    
    for idx, test_mode in enumerate(test_modes):
        ax = axes[idx]
        
        mode_data = df[df['TestMode'] == test_mode]
        
        for i, algo in enumerate(algorithms):
            algo_data = mode_data[mode_data['Algorithm'] == algo]
            ax.plot(algo_data['N'], algo_data['Runtime(ms)'], 
                   label=algo, 
                   color=colors[i % len(colors)],
                   marker=markers[i % len(markers)],
                   linewidth=2,
                   markersize=4)
        ax.set_title(f'Test Mode: {test_mode}', fontsize=12, fontweight='bold')
        ax.set_xlabel('Number of Rectangles (n)', fontsize=11)
        ax.set_ylabel('Runtime (ms)', fontsize=11)
        ax.legend(loc='upper left', fontsize=9)
        ax.grid(True, linestyle='--', alpha=0.5)
    
    plt.suptitle('Texture Packing Algorithm Performance Comparison', 
                 fontsize=14, fontweight='bold', y=1.02)
    plt.tight_layout()
    
    # save the photo
    plt.savefig('runtime_comparison.png', dpi=300, bbox_inches='tight')
    print("Saved runtime_comparison.png")
    plt.show()

if __name__ == "__main__":
    main()
