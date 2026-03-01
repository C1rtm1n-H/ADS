
import pandas as pd
import matplotlib.pyplot as plt


def main():
    # read the csv file
    df = pd.read_csv('results/result.csv')

    required_cols = {'Algorithm', 'TestMode', 'N', 'FillRatio'}
    if not required_cols.issubset(df.columns):
        missing = required_cols - set(df.columns)
        raise ValueError(f"result.csv 缺少必要列: {missing}")

    test_modes = list(dict.fromkeys(df['TestMode']))
    algorithms = list(dict.fromkeys(df['Algorithm']))

    colors = ['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728', '#9467bd']
    markers = ['o', 's', '^', 'D', 'v']
    fig, axes = plt.subplots(1, 3, figsize=(18, 6))

    for idx, test_mode in enumerate(test_modes):
        ax = axes[idx]

        mode_data = df[df['TestMode'] == test_mode]
        for i, algo in enumerate(algorithms):
            algo_data = mode_data[mode_data['Algorithm'] == algo]
            algo_data = algo_data.sort_values('N')
            ax.plot(
                algo_data['N'],
                algo_data['FillRatio'],
                label=algo,
                color=colors[i % len(colors)],
                marker=markers[i % len(markers)],
                linewidth=2,
                markersize=4,
            )
        ax.set_title(f'Test Mode: {test_mode}', fontsize=12, fontweight='bold')
        ax.set_xlabel('Number of Rectangles (n)', fontsize=11)
        ax.set_ylabel('Fill Ratio', fontsize=11)
        ax.set_ylim(0, 1.05)
        ax.grid(True, linestyle='--', alpha=0.5)
        ax.legend(loc='lower right', fontsize=9)

    plt.suptitle('Texture Packing Fill Ratio Comparison', fontsize=14, fontweight='bold', y=1.02)
    plt.tight_layout()

    plt.savefig('fillratio_comparison.png', dpi=300, bbox_inches='tight')
    print('Saved fillratio_comparison.png')
    plt.show()


if __name__ == '__main__':
    main()
