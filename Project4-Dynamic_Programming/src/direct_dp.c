#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MOD 1000000007
#define MAX_N 100010
#define MAX_VAL 100010

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

// 计算beautiful subsequences的数量（简单版本）
// 参数: n - 数组长度, m - 差值限制, arr - 输入数组
// 返回: beautiful subsequences的数量
long long simple_version_timed(int n, int m, int arr[]) {
    long long dp[MAX_VAL];
    for (int i = 0; i < n; i++) {
        dp[i] = 0;
    }
    
    long long sum = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (abs(arr[i] - arr[j]) <= m) {
                dp[i] = (dp[i] + power2(j)) % MOD;
            }
            else {
                dp[i] = (dp[i] + dp[j]) % MOD;
            }
        }
        sum = (sum + dp[i]) % MOD;
    }
    
    return sum;
}
