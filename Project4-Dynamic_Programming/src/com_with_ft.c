#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MOD 1000000007
#define MAX_N 100010
#define MAX_VAL 100010

long long tree[MAX_VAL];

// Update the tree array for Fenwick Tree (Binary Indexed Tree)
void update(int idx, long long val) {
    while (idx < MAX_VAL) {
        tree[idx] = (tree[idx] + val) % MOD;
        idx += idx & -idx;
    }
}

// Query the sum from 1 to idx in the Fenwick Tree
long long query(int idx) {
    long long sum = 0;
    while (idx > 0) {
        sum = (sum + tree[idx]) % MOD;
        idx -= idx & -idx;
    }
    return sum;
}

static long long power2(int n) {
    long long result = 1;
    long long base = 2;
    while (n > 0) {
        if (n & 1) {
            result = (base * result) % MOD;
        }
        base = (base * base) % MOD;
        n >>= 1;
    }
    return result;
}

// 计算beautiful subsequences的数量
// 参数: n - 数组长度, m - 差值限制, arr - 输入数组
// 返回: beautiful subsequences的数量
long long beautiful_subsequence_timed(int n, int m, int arr[]) {
    memset(tree, 0, sizeof(tree));
    
    // dp[i] refers to the number of "ugly" subsequences ending with arr[i]
    long long total_ugly = 0;
    
    for (int i = 0; i < n; i++) {
        // Calculate the number of subsequences that can be appended before arr[i]
        // The last element of these subsequences must differ from arr[i] by more than m
        int low = (arr[i] > m + 1) ? arr[i] - m - 1 : 0;
        int high = (arr[i] + m + 1 < MAX_VAL) ? arr[i] + m + 1 : MAX_VAL - 1;

        // The number of subsequences that can be appended before arr[i] =
        // the sum of all subsequences ending with values in [1, low] or [high, MAX_VAL-1]
        long long count = (query(low) + query(MAX_VAL - 1) - query(high - 1) + MOD) % MOD;
        
        // ugly subsequences ending with arr[i]
        long long new_ugly_i = (1 + count) % MOD;
        
        // update the Fenwick Tree
        update(arr[i], new_ugly_i);
        
        total_ugly = (total_ugly + new_ugly_i) % MOD;
    }
    
    // total subsequences (excluding empty subsequence) = 2^n - 1
    long long total_subsequences = (power2(n) - 1 + MOD) % MOD;
    
    // total ugly subsequences
    long long ugly_subsequences = (total_ugly + MOD) % MOD;
    
    // beautiful subsequences = total subsequences - ugly subsequences
    long long beautiful_subsequences = (total_subsequences - ugly_subsequences + MOD) % MOD;
    
    return beautiful_subsequences;
}
