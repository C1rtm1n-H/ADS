#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MOD 1000000007
#define MAX_VAL 100010

long long power(long long base, long long exp) {
    long long res = 1;
    base %= MOD;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % MOD;
        base = (base * base) % MOD;
        exp /= 2;
    }
    return res;
}

long long complementary_no_ft_timed(int n, int m, int arr[]) {
    long long* dp = (long long*)calloc(MAX_VAL, sizeof(long long));
    if (!dp) return -1;

    long long total_ugly = 0;

    for (int i = 0; i < n; i++) {
        long long sum_prev_ugly = 0;
        // 朴素循环代替树状数组
        for (int v = 1; v < arr[i] - m; v++) {
            sum_prev_ugly = (sum_prev_ugly + dp[v]) % MOD;
        }
        for (int v = arr[i] + m + 1; v < MAX_VAL; v++) {
            sum_prev_ugly = (sum_prev_ugly + dp[v]) % MOD;
        }

        long long new_ugly_at_val = (1 + sum_prev_ugly) % MOD;
        dp[arr[i]] = (dp[arr[i]] + new_ugly_at_val) % MOD;
        total_ugly = (total_ugly + new_ugly_at_val) % MOD;
    }

    long long total_subsequences_len_ge_2 = (power(2, n) - 1 - n + MOD + MOD) % MOD;
    long long beautiful_subsequences = (total_subsequences_len_ge_2 - total_ugly + MOD) % MOD;

    free(dp);
    return beautiful_subsequences;
}