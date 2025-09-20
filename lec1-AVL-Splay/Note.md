# AVL
- **AVL树的高度**
    - 默认空树高度为-1，叶节点高度为0
    - 最坏情况下的高度为O(log n)
    - 通过递归关系n(h) = n(h-1) + n(h-2) + 1 = Θ(φ^h)，其中φ为黄金分割率，n(h)表示高度为h的AVL树的最小节点数
    - n(h) = F(h+3) - 1，其中F为斐波那契数列，h>=0

- **Implementation of height**
    ```c
    int height(TreeNode* node){
        return node ? node->height : 0;
    }
    ```
- **Implementation of balance factor**
    ```c
    int getBalance(TreeNode* node){
        return node ? height(node->left) - height(node->right) : 0;
    }
    ```
- **Implementation of rotation**
    ```c
    TreeNode* rotationRight(TreeNode* y){
        TreeNode* x = y->left;
        TreeNode* T2 = x->right;
        //perform rotation
        x->right = y;
        y->left = T2;
        //update height
        y->height = 1 + MAX(height(y->left), height(y->right));
        x->height = 1 + MAX(height(x->left), height(x->right));
        //return new root
        return x;
    }
    ```
- **Implementation of insertion**
    ```c
    TreeNode* insert(TreeNode* node, int key){
        // 1.normal BST insertion
        if(!node) return CreateNode(key);
        if(key < node->key){
            node->left = insert(node->left, key);
        }
        else{
            node->right = insert(node->right, key);
        }
        // 2.update height of this ancestor node
        node->height = 1 + MAX(height(node->left), height(node->right));
        // 3.get the balance factor of this ancestor node to check whether
        // this node became unbalanced
        int balance = getBalance(node);
        // if this node becomes unbalanced, then there are 4 cases
        // Left Left Case
        if(balance > 1 && key < node->left->key){
            return rotationRight(node);
        }
        // Left Right Case
        if(nalance > 1 && key > node->left->key){
            node->left = rotationLeft(node->left);
            return rotationRight(node);
        }
        // Right Right Case
        if(balance < -1 && key > node->right->key){
            return rotationLeft(node);
        }
        // Right Left Case
        if(balance < -1 && key < node->right->key){
            node->right = rotationRight(node->right);
            return rotationLeft(node);
        }
    }
# Splay
- **Splay操作**
    - zig：单次Single Rotation
    - zig-zig：对根节点两次Single Rotation
    - zig-zag：一次Double Rotation，即先对child节点进行Single Rotation，再对根节点进行Single Rotation
    - deletion: 先splay要删除的节点到根节点，然后删除根节点，再将左子树的最大节点splay到左子树的根节点，最后将右子树接到左子树的右子节点上
- **实现Splay操作**
    ```c
    TreeNode* splay(TreeNode* root, int key){
        // Base cases: root is NULL or key is present at root
        if(root == NULL || root->key == key) return root;
        // Key lies in left subtree
        if(key < root->key){
            // Key is not in tree, we are done
            if(root->left == NULL) return root;
            //Zig-Zig (LL)
            if(key < root->left->key){
                // First recursively bring the key as root of left-left
                root->left->left = splay(root->left->left, key);
                // Do first rotation for root, second rotation is done after else
                root = rotationRight(root);
                // Now do the second rotation
                root = rotationRight(root);
            }
            // Zig-Zag (LR)
            else if(key > root->left->key){
                // First recursively bring the key as root of left-right
                root->left->right = splay(root->left->right, key);
                // Do first rotation for root->left
                if(root->left->right != NULL)
                    root->left = rotationLeft(root->left);
                // Now do second rotation for root
                root = rotationRight(root);
            }
        }
        else{ // Key lies in right subtree
            // Key is not in tree, we are done
            if(root->right == NULL) return root;
            // Zag-Zig (RL)
            if(key < root->right->key){
                // Bring the key as root of right-left
                root->right->left = splay(root->right->left, key);
                // Do first rotation for root->right
                if(root->right->left != NULL)
                    root->right = rotationRight(root->right);
                // Do second rotation for root
                root = rotationLeft(root);
            }
            // Zag-Zag (RR)
            else{
                // Bring the key as root of right-right and do first rotation
                root->right->right = splay(root->right->right, key);
                root = rotationLeft(root);
                // Do second rotation for root
                root = rotationLeft(root);
            }
        }

## Amortized Analysis（均摊分析）
- **Aggregate Method（聚合方法）**
    n次操作总的worst时间为T(n)，均摊时间为T(n)/n
- **Accounting Method（核算法）**
    - 真实花销c_i
    - 某些c_i'大于c_i，某些c_i'小于c_i，多的作为credit，用来支付少的
    - 保证credit不为负
    > 栈操作： push的均摊时间为2，pop的均摊时间为0，因为push多花1单位时间作为credit存起来，pop时用这个credit支付。均摊时间的总和是实际时间的上界。
- **Potential Function（势能函数）**
    - 定义系统状态的函数Φ(D)，势能函数反应数据系统的复杂度
    - 第i次操作的均摊时间为c_i + Φ(D_i) - Φ(D_{i-1})
    - 保证Φ(D_i) ≥ Φ(D_0)，通常取Φ(D_0) = 0
