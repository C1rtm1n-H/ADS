import pandas as pd
import matplotlib.pyplot as plt

def plot_scale():
    try:
        df = pd.read_csv("test/benchmark_scale.csv")
    except FileNotFoundError:
        print("benchmark_scale.csv not found. Run benchmark.py first.")
        return

    plt.figure(figsize=(10, 6))
    plt.plot(df["size"], df["serial_time"], marker='o', label="Serial")
    plt.plot(df["size"], df["parallel_time"], marker='s', label="Parallel (4 Threads)")
    plt.xlabel("Input Size (Number of Words)")
    plt.ylabel("Time (seconds)")
    plt.title("Performance Comparison: Serial vs Parallel")
    plt.legend()
    plt.grid(True)
    plt.savefig("test/plot_scale.png")
    print("Saved test/plot_scale.png")

def plot_threads():
    try:
        df = pd.read_csv("test/benchmark_threads.csv")
    except FileNotFoundError:
        print("benchmark_threads.csv not found. Run benchmark.py first.")
        return

    plt.figure(figsize=(10, 6))
    plt.plot(df["threads"], df["time"], marker='o', color='green')
    plt.xlabel("Number of Threads (Mappers/Reducers)")
    plt.ylabel("Time (seconds)")
    plt.title("Parallel Performance vs Thread Count (Size=5M)")
    plt.grid(True)
    plt.xticks(df["threads"])
    plt.savefig("test/plot_threads.png")
    print("Saved test/plot_threads.png")

if __name__ == "__main__":
    plot_scale()
    plot_threads()