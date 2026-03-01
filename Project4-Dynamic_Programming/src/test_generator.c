#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void generate_test_data(int n, int max_value, int arr[]) {
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % max_value + 1;
    }
}

