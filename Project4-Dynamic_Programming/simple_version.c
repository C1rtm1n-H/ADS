#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MOD 1000000007
#define MAX_N 100010
#define MAX_VAL 100010

int n, m;
int a[MAX_N];
//the implementation of fast exponentiation
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
    //dp[i] store the number of the subsequences ending with a[i]
    long long dp[MAX_N];
    for (int i = 0; i < n; i++) {
        scanf("%d", &a[i]);
        dp[i] = 0;
    }
    long long sum = 0;
   for (int i = 0; i < n; i++) {
    for (int j = 0; j < i; j++) {
        if (abs(a[i] - a[j]) <= m) {
            //if |a[i] - a[j]| <= m, already satify the condition,no matter what we choose before a[j],it fits.//
            dp[i] = (dp[i] + power2(j)) % MOD;
        }
        else {
            //if |a[i] - a[j]| > m, then the subsequence before a[i] must be a beautiful subseqence//
            dp[i] = (dp[i] + dp[j]) % MOD;
        }
    }
    sum = (sum + dp[i]) % MOD;
   }
   printf("%lld \n %ld", sum);
   return 0;
}