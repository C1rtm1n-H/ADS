## AVL

## Splay
- **Splay操作**
    - zig：单次Single Rotation
    - zig-zig：特殊的Single Rotation
        - 并非一个single rotation做两次，而是将三个节点直接颠倒
    - zig-zag：一次Double Rotation

## Amortized Analysis
- **Aggregate Method**
    总的worst时间为T(n)，均摊时间为T(n)/n
- **Accounting Method**
    - 真实花销c_i
    - 某些c_i'大于c_i，某些c_i'小于c_i，多的作为credit，用来支付少的
    - 保证credit不为负
- **Potential Function**
    - 定义系统状态的函数Φ(D)，势能函数反应数据系统的复杂度
    - 第i次操作的均摊时间为c_i + Φ(D_i) - Φ(D_{i-1})
    - 保证Φ(D_i) ≥ Φ(D_0)，通常取Φ(D_0) = 0
