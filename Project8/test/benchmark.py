import subprocess
import time
import os
import csv

# 配置
DATA_SIZES = [10000, 50000, 100000, 200000, 500000, 1000000, 2000000, 5000000]
THREAD_COUNTS = [1, 2, 4, 8]
ITERATIONS = 3  # 每个测试运行3次取平均

def run_command(cmd, env=None):
    start_time = time.time()
    subprocess.run(cmd, shell=True, check=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, env=env)
    end_time = time.time()
    return end_time - start_time

def benchmark_scale():
    print("Benchmarking Scale (Time vs Input Size)...")
    results = []
    
    for size in DATA_SIZES:
        input_file = f"test/test_data_{size}.txt"
        print(f"  Testing size {size}...")
        
        # Serial
        serial_times = []
        for _ in range(ITERATIONS):
            t = run_command(f"./serial_wc test/out_s_{size}.txt {input_file}")
            serial_times.append(t)
        avg_serial = sum(serial_times) / len(serial_times)
        
        # Parallel (4 threads)
        parallel_times = []
        env = os.environ.copy()
        env["NUM_MAPPERS"] = "4"
        env["NUM_REDUCERS"] = "4"
        for _ in range(ITERATIONS):
            t = run_command(f"./parallel_wc test/out_p_{size}.txt {input_file}", env)
            parallel_times.append(t)
        avg_parallel = sum(parallel_times) / len(parallel_times)
        
        results.append({
            "size": size,
            "serial_time": avg_serial,
            "parallel_time": avg_parallel
        })
        
    # Save results
    with open("test/benchmark_scale.csv", "w", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=["size", "serial_time", "parallel_time"])
        writer.writeheader()
        writer.writerows(results)
    print("Scale benchmark saved to test/benchmark_scale.csv")

def benchmark_threads():
    print("Benchmarking Threads (Time vs Thread Count)...")
    results = []
    size = 5000000 # Use largest dataset
    input_file = f"test/test_data_{size}.txt"
    
    for threads in THREAD_COUNTS:
        print(f"  Testing {threads} threads...")
        
        times = []
        env = os.environ.copy()
        env["NUM_MAPPERS"] = str(threads)
        env["NUM_REDUCERS"] = str(threads)
        
        for _ in range(ITERATIONS):
            t = run_command(f"./parallel_wc test/out_t_{threads}.txt {input_file}", env)
            times.append(t)
        avg_time = sum(times) / len(times)
        
        results.append({
            "threads": threads,
            "time": avg_time
        })
        
    # Save results
    with open("test/benchmark_threads.csv", "w", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=["threads", "time"])
        writer.writeheader()
        writer.writerows(results)
    print("Thread benchmark saved to test/benchmark_threads.csv")

if __name__ == "__main__":
    # Ensure executables exist
    subprocess.run("make", shell=True, check=True)
    # Ensure test data exists
    subprocess.run("make testdata", shell=True, check=True)
    
    benchmark_scale()
    benchmark_threads()