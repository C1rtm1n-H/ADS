#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MOD 1000000007

long long count = 0;
int n_global, m_global;
int* a_global;
int* current_sub;

// 检查当前子序列是否美丽
bool is_beautiful(int len) {
    if (len < 2) return false;
    for (int i = 0; i < len - 1; i++) {
        if (abs(current_sub[i] - current_sub[i+1]) <= m_global) {
            return true;
        }
    }
    return false;
}

// 递归生成子序列
void solve(int index, int len) {
    if (index == n_global) {
        if (is_beautiful(len)) {
            count = (count + 1) % MOD;
        }
        return;
    }

    // Case 1: 不包含 a_global[index]
    solve(index + 1, len);

    // Case 2: 包含 a_global[index]
    current_sub[len] = a_global[index];
    solve(index + 1, len + 1);
}


long long brute_force_timed(int n, int m, int arr[]) {
    n_global = n;
    m_global = m;
    a_global = arr;
    current_sub = (int*)malloc(n * sizeof(int));
    count = 0;
    
    // 我们需要排除空集和单元素集，但通过递归生成所有子集再判断更简单
    // 最终结果需要减去所有非美丽子序列（包括空和单元素）
    // 一个更简单的思路是：总数 - 丑陋数。但暴力法直接数美丽数更直观。
    // is_beautiful 函数已经处理了长度小于2的情况。
    solve(0, 0);
    
    free(current_sub);
    return count;
}