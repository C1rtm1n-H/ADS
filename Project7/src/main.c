#include "skip_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_N 1000000
#define MIN_REPETITIONS 5
#define TARGET_OPS 500000

// compute the number of repetitions based on n
#define GET_REPETITIONS(n) ((n) <= (TARGET_OPS / MIN_REPETITIONS) ? (TARGET_OPS / (n)) : MIN_REPETITIONS)

// different operations function pointer
typedef void (*OperationFunc)(int *values, int n, int repetitions);

// save the result to the csv file
void store_result(const char *operation, int n, double avg_time, int repetitions, const char *filename) {
    FILE *f = fopen(filename, "a");
    if (f == NULL) {
        perror("无法打开文件");
        return;
    }
    fprintf(f, "%s,%d,%.6f\n", operation, n, avg_time);
    fclose(f);
}

// initialize CSV file with header
void init_csv(const char *filename) {
    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        perror("无法打开文件");
        return;
    }
    fprintf(f, "Operation,N,Time(ms)\n");
    fclose(f);
}

// generate a random permutation of 1 to n
void generate_random_permutation(int *arr, int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = i + 1;  
    }
    // Fisher-Yates shuffle
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

// test insert operation
void test_insert(int *values, int n, int repetitions) {
    //start timing
    clock_t start = clock();
    // repeat for 'repetitions' times
    for (int r = 0; r < repetitions; r++) {
        skip_list *local_list = skip_list_create();
        for (int i = 0; i < n; i++) {
            skip_list_insert(local_list, values[i]);
        }
        skip_list_free(local_list);
    }
    // end timing
    clock_t end = clock();
    double total_time = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
    double avg_time = total_time / repetitions;
    //store the result to the csv file
    store_result("insert", n, avg_time, repetitions, "results/skip_list_data.csv");
    printf("    insert: n=%d, repetitions=%d, avg_time=%.4f ms\n", n, repetitions, avg_time);
}

// test find operation
void test_find(int *values, int n, int repetitions) {
    // create a skip list and insert all values
    skip_list *list = skip_list_create();
    for (int i = 0; i < n; i++) {
        skip_list_insert(list, values[i]);
    }
    // start timing
    clock_t start = clock();
    // repeat for 'repetitions' times
    for (int r = 0; r < repetitions; r++) {
        for (int i = 0; i < n; i++) {
            skip_list_find(list, values[i]);
        }
    }
    // end timing
    clock_t end = clock();
    double total_time = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
    double avg_time = total_time / repetitions;
    // store the result to the csv file
    store_result("find", n, avg_time, repetitions, "results/skip_list_data.csv");
    printf("    find:   n=%d, repetitions=%d, avg_time=%.4f ms\n", n, repetitions, avg_time);

    skip_list_free(list);
}

// test delete operation 
void test_delete(int *values, int n, int repetitions) {
    // build repetitions copies of skip lists for deletion
    skip_list **copies = (skip_list **)malloc(repetitions * sizeof(skip_list *));
    if (copies == NULL) {
        fprintf(stderr, "内存分配失败\n");
        return;
    }
    // create and insert value into  skip lists of 'repetitions' copies
    for (int r = 0; r < repetitions; r++) {
        copies[r] = skip_list_create();
        for (int i = 0; i < n; i++) {
            skip_list_insert(copies[r], values[i]);
        }
    }
    // start timing
    clock_t start = clock();
    for (int r = 0; r < repetitions; r++) {
        for (int i = 0; i < n; i++) {
            skip_list_delete(copies[r], values[i]);
        }
    }
    // end timing
    clock_t end = clock();
    
    double total_time = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
    double avg_time = total_time / repetitions;
    // store the result to the csv file
    store_result("delete", n, avg_time, repetitions, "results/skip_list_data.csv");
    printf("    delete: n=%d, repetitions=%d, avg_time=%.4f ms\n", n, repetitions, avg_time);
    
    // free the skip lists
    for (int r = 0; r < repetitions; r++) {
        skip_list_free(copies[r]);
    }
    free(copies);
}

int main() {
    srand((unsigned int)time(NULL));  // seed the random number generator

    // the different test scale ,from 1000 to 1000000
    int test_n[] = {1000, 5000, 10000, 25000, 50000, 75000, 100000, 
                    200000, 300000, 400000, 500000, 750000, 1000000};
    int num_tests = sizeof(test_n) / sizeof(test_n[0]);

    // operation function
    const char *operation_names[] = {"insert", "find", "delete"};
    OperationFunc operations[] = {test_insert, test_find, test_delete};
    int num_operations = 3;

    // initialize the csv file
    init_csv("results/skip_list_data.csv");

    //test for different test scale n
    for (int i = 0; i < num_tests; i++) {
        int n = test_n[i];
        printf("Testing n = %d ...\n", n);

        int *values = (int *)malloc(n * sizeof(int));
        if (values == NULL) {
            fprintf(stderr, "内存分配失败\n");
            return 1;
        }
        generate_random_permutation(values, n);

        // different funtions test
        for (int op = 0; op < num_operations; op++) {
            int repetitions = GET_REPETITIONS(n);
            operations[op](values, n, repetitions);
        }

        free(values);
        printf("\n");
    }

    printf("性能测试完成！结果已保存到 results/skip_list_data.csv\n");
    return 0;
}
