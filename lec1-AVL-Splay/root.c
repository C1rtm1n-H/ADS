#include<stdio.h>
#include<stdlib.h>
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

typedef struct TreeNode{
    int val;
    int ht;
    struct TreeNode* left;
    struct TreeNode* right;
}TreeNode;

TreeNode* CreateNode(int val){
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->val = val;
    node->ht = 0;
    node->left = NULL;
    node->right = NULL;
    return node;    //返回指向创建节点的指针
}

int height(TreeNode* node){
    return node ? node->ht : -1;    //空树高度为-1
}

void UpdateHeight(TreeNode* node){   //更新并返回整数的高度
    /*逻辑错误，update不需要递归，每次插入和旋转操作都进行update，使得左右高度差不会大于一
    if(node == NULL) return;
    if(node->left == NULL && node->right == NULL) return 0;  //叶子
    else if(node->left == NULL) node->ht = 1 + UpdateHeight(node->right);  //左空
    else if(node->right == NULL) node->ht = 1 + UpdateHeight(node->left);  //右空
    else node->ht = 1 + MAX(UpdateHeight(node->left), UpdateHeight(node->right));    //左右都不空，取较高
    */
    if(node) node->ht = 1 + MAX(height(node->left), height(node->right));
}

TreeNode* RotateL(TreeNode* node){   //node是我，bf不符合要求的节点
    TreeNode* tmp1 = node->right;   
    TreeNode* tmp2 = tmp1->left;
    tmp1->left = node;          //儿子变爸爸
    node->right = tmp2;         //孙子变儿子

    UpdateHeight(node);         //从下向上更新高度
    UpdateHeight(tmp1);

    return tmp1;                //返回新根，以便于重新向上连接
}

TreeNode* RotateR(TreeNode* node){
    TreeNode* tmp1 = node->left;
    TreeNode* tmp2 = tmp1->right;
    tmp1->right = node;
    node->left = tmp2;

    UpdateHeight(node);
    UpdateHeight(tmp1);

    return tmp1;
}

TreeNode* Insert(TreeNode* node, int val){
    //跳出条件，返回指向插入节点的指针
    if(node==NULL){
        return node = CreateNode(val);
    }
    //递归插入
    if(val < node->val){
        node->left = Insert(node->left, val);
    }
    else{
        node->right = Insert(node->right, val);
    }
    UpdateHeight(node);

    //每次回归后执行
    int bf = (node) ? (height(node->left) - height(node->right)) : 0;
    if(bf == 0) return node;
    if(bf > 1){ //L
        if(val < node->left->val){    //LL
            node = RotateR(node);     //node节点保留有与父节点的连接，如此操作父节点可以指向新根
        }
        else{   //LR
            node->left = RotateL(node->left);
            node = RotateR(node);
        }
    }
    if(bf < -1){ //R
        if(val > node->right->val){ //RR
            node = RotateL(node);
        }
        else{   //RL
            node->right = RotateR(node->right);
            node = RotateL(node);    
        }

    }

    return node;
}

int main()
{
    int n;
    scanf("%d", &n);
    TreeNode* root=NULL;
    for(int i=0; i<n; i++){
        int val;
        scanf("%d", &val);
        root = Insert(root, val);
    }
    printf("%d", root->val);

    return 0;
}

/*
方法十分困难，作废
int main(){
    int n, nodes[MAXN]={0}, height[MAXN]={0}, bf[MAXN]={0};

    scanf("%d", &n);

    for(int i=0; i<n; i++){
        int idx=1, er_idx=0;
        // insert
        if(i=0){
            scanf("%d", &nodes[idx]);
            continue;
        }
        else{
            int tmp;
            scanf("%d", &tmp);
            while(nodes[idx] != 0){
                if(tmp < nodes[idx]) idx = idx*2;
                else idx = idx*2 + 1;
            }
            nodes[idx] = tmp;
        }

        // update height and bf, find closest error index
        for(int j=idx; j>=1; j/=2){
            height[j/2] = (height[j] > height[j+1]) ? height[j] : height[j+1];
            bf[j/2] = height[j] - height[j+1];
            if(bf[j/2]<-1 || bf[j/2]>1){
                er_idx = (j/2 > er_idx) ? j/2 : er_idx; 
            }
        }

        // rotate
        if(bf[er_idx] > 0){ //L
            if(bf[er_idx*2 > 0]){   //LL
                //先储存出错节点的值
                int tmp_node = nodes[er_idx];
                //er_idx*2的节点以及其左子树上移，右子树根为er_idx*4+1
                for(int j=er_idx; nodes[j]>0; j*=2){
                    nodes[j] = nodes[j*2]

                }
            }
            else{   //LR

            }
        }

        else{   //R
            if(bf[er_idx*2 < 0]){   //RR

            }
            else{   //RL

            }
        }
    }

    int root = nodes[1];
    printf("%d", root);
    return 0;
}
*/

