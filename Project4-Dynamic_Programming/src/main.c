#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define REPEATITION 100
#define MAX_VAL 100010

// the declaration of the external functions//
long long beautiful_subsequence_timed(int n, int m, int arr[]);
long long simple_version_timed(int n, int m, int arr[]);
long long brute_force_timed(int n, int m, int arr[]);
long long complementary_no_ft_timed(int n, int m, int arr[]);
void generate_test_data(int n, int max_value, int arr[]);

//store function//
void store(char* TestAlgorithm, int n, double runtime, const char* filename) {
    FILE* f = fopen(filename, "a");
    if (f == NULL) {
        perror("can't open the file");
        return;
    }
    fprintf(f, "%s,%d,%lf\n",TestAlgorithm, n, runtime);
    fclose(f);
}
//init function//
void init(char* filename) {
    FILE* f = fopen(filename, "w");
    if (f == NULL) {
        perror("can't open the file");
        return;
    }
    fprintf(f, "TestAlgorithm,N,Runtime(ms)\n");
    fclose(f);
}
int main() {
    srand(time(NULL));  //the random seed//
    init("result.csv");
    
    // test for algorithm1 (O(N log V))
    printf("Testing Algorithm 1 (Optimized DP O(N log V))...\n");
    for (int n = 10000; n <= 100000; n+=10000) { // 减少数据点以加快测试
        int *input_array = (int *)malloc(n * sizeof(int));
        generate_test_data(n, MAX_VAL, input_array);
        clock_t start, end;
        start = clock();
        for (int j = 0; j < REPEATITION; j++) {
            (void)beautiful_subsequence_timed(n, (MAX_VAL + 1)/3, input_array);
        }
        end = clock();
        double runtime = ((double)(end - start)) / CLOCKS_PER_SEC / REPEATITION * 1000;
        free(input_array);
        store("Algorithm1_OptimizedDP", n, runtime, "result.csv");
        printf("N = %d, Runtime = %lf ms\n", n, runtime);
    }

    // test for algorithm2 (O(N^2))
    printf("\nTesting Algorithm 2 (Direct DP O(N^2))...\n");
    for (int n = 500; n <= 5000; n+=500) {
        int *input_array = (int *)malloc(n * sizeof(int));
        generate_test_data(n, MAX_VAL, input_array);
        clock_t start, end;
        start = clock();
        for (int j = 0; j < 10; j++) {
            (void)simple_version_timed(n, (MAX_VAL + 1)/3, input_array);
        }
        end = clock();
        double runtime = ((double)(end - start)) / CLOCKS_PER_SEC / 10 * 1000;
        free(input_array);
        store("Algorithm2_DirectDP", n, runtime, "result.csv");
        printf("N = %d, Runtime = %lf ms\n", n, runtime);
    }

    // test for algorithm3 (O(N*V))
    printf("\nTesting Algorithm 3 (Complementary DP O(N*V))...\n");
    for (int n = 500; n <= 5000; n+=500) {
        int *input_array = (int *)malloc(n * sizeof(int));
        generate_test_data(n, MAX_VAL, input_array);
        clock_t start, end;
        start = clock();
        // O(N*V) 会非常慢，只执行一次
        (void)complementary_no_ft_timed(n, (MAX_VAL + 1)/3, input_array);
        end = clock();
        double runtime = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;
        free(input_array);
        store("Algorithm3_ComplementaryDP", n, runtime, "result.csv");
        printf("N = %d, Runtime = %lf ms\n", n, runtime);
    }

    // test for algorithm4 (O(2^N))
    printf("\nTesting Algorithm 4 (Brute Force O(2^N))...\n");
    for (int n = 10; n <= 22; n+=2) {
        int *input_array = (int *)malloc(n * sizeof(int));
        generate_test_data(n, MAX_VAL, input_array);
        clock_t start, end;
        start = clock();
        for(int j = 0; j < REPEATITION; j++) {
            (void)brute_force_timed(n, (MAX_VAL + 1)/3, input_array);
        }
        end = clock();
        double runtime = ((double)(end - start)) / CLOCKS_PER_SEC / REPEATITION * 1000;
        free(input_array);
        store("Algorithm4_BruteForce", n, runtime, "result.csv");
        printf("N = %d, Runtime = %lf ms\n", n, runtime);
    }

    printf("\nAll tests completed. Results are in result.csv\n");
    return 0;
}