#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MOD 1000000007
#define MAX_N 100010
#define MAX_VAL 100010

int n, m;
int a[MAX_N];
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
long long power2(int n) {
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
int main() {
    scanf("%d %d", &n, &m);
    for (int i = 0; i < n; i++) {
        scanf("%d", &a[i]);
    }
    
    memset(tree, 0, sizeof(tree));
    
    // dp[i] refers to the number of "ugly" subsequences ending with a[i]
    long long total_ugly = 0;
    
    for (int i = 0; i < n; i++) {
        // Calculate the number of subsequences that can be appended before a[i]
        // The last element of these subsequences must differ from a[i] by more than m
        int low = (a[i] > m + 1) ? a[i] - m - 1 : 0;
        int high = (a[i] + m + 1 < MAX_VAL) ? a[i] + m + 1 : MAX_VAL - 1;

        // The number of subsequences that can be appended before a[i] =
        // the sum of all subsequences ending with values in [1, low] or [high, MAX_VAL-1]
        long long count = (query(low) + query(MAX_VAL - 1) - query(high - 1) + MOD) % MOD;
        
        // ugly subsequences ending with a[i]
        long long new_ugly_i = (1 + count) % MOD;
        
        // update the Fenwick Tree
        update(a[i], new_ugly_i);
        
        total_ugly = (total_ugly + new_ugly_i) % MOD;
    }
    
    // total subsequences (excluding empty subsequence) = 2^n - 1 - n
    long long total_subsequences = (power2(n) - 1 + MOD) % MOD;
    
    // total ugly subsequences
    long long ugly_subsequences = (total_ugly + MOD) % MOD;
    
    // beautiful subsequences = total subsequences - ugly subsequences
    long long beautiful_subsequences = (total_subsequences - ugly_subsequences + MOD) % MOD;
    printf("%lld\n", beautiful_subsequences);
       
    return 0;
}