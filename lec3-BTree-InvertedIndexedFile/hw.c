#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

typedef struct Node{
    int key[4];
    int num_keys;
    struct Node* child[4];
    struct Node* parent;
    bool is_leaf;
}Node;

Node* createNode(bool is_leaf, Node* parent);
Node* findLeaf(int key, Node* root);
Node* insertIntoLeaf(Node* leaf, int key);
Node* insertIntoParent(Node* parent, int key);
Node* splitNode(bool is_leaf, Node* node);
Node* insert(int key, Node* root);
void traverse(Node* root);

Node* createNode(bool is_leaf, Node* parent){
    Node* new_node = (Node*)malloc(sizeof(Node));
    if(!new_node) {
        perror("Malloc failed");
        exit(EXIT_FAILURE);
    }
    new_node->is_leaf = is_leaf;
    new_node->num_keys = 0;
    new_node->parent = parent;
    for(int i=0; i<4; i++) new_node->key[i] = -1;   //初始化为-1表示无效
    for(int i=0; i<4; i++) new_node->child[i] = NULL;
    return new_node;
}

Node* findLeaf(int key, Node* root){//寻找key应该插入的叶子节点
    if(root == NULL) return NULL;
    Node* current = root;
    while(current->is_leaf == false){
        int i=0;
        while(i < current->num_keys && key > current->key[i]) i++;//找到第一个大于key的key的位置
        current = current->child[i];
    }
    return current;
}

//key少于3直接插入，key等于3插入后分裂
Node* insertIntoLeaf(Node* leaf, int key){
    int i=0;
    while(i < leaf->num_keys && key > leaf->key[i]) i++;
    for(int j=leaf->num_keys; j>i; j--){//插入排序
        leaf->key[j] = leaf->key[j-1];
    }
    leaf->key[i] = key;
    leaf->num_keys++;

    if(leaf->num_keys <= 3){
        return NULL; //根节点没有改变
    }
    else{//叶子节点满了，分裂
        return splitNode(true, leaf);
    }
}

Node* insertIntoParent(Node* parent, int key){
    if(parent == NULL){
        Node* new_node = createNode(false, NULL);
        new_node->key[0] = key;
        new_node->num_keys = 1;
        return new_node;
    }else{
        int i=0;
        while(i < parent->num_keys && key > parent->key[i]) i++;
        for(int j=parent->num_keys; j>i; j--){//插入排序
            parent->key[j] = parent->key[j-1];
        }
        parent->key[i] = key;
        parent->num_keys++;

        if(parent->num_keys <= 2){
            return NULL; //根节点没有改变
        }else{//父节点满了，分裂
            return splitNode(false, parent);
        }
    }
}
//分裂有4个key的叶节点或者有3个key的非叶节点
Node* splitNode(bool is_leaf, Node* node){
    Node* parent = node->parent;
    Node* new_node = createNode(is_leaf, node->parent);
    int pass_key;
    if(is_leaf){
        // 叶子节点有4个键: [k0, k1, k2, k3]
        // 分裂成 [k0, k1] 和 [k2, k3]
        // 上推 k2
        pass_key = node->key[2];
        new_node->key[0] = node->key[2];
        new_node->key[1] = node->key[3];
        new_node->num_keys = 2;
        node->key[2] = -1;
        node->key[3] = -1;
        node->num_keys = 2;
        
    }else{
        // 内部节点有3个键: [k0, k1, k2] 和4个子节点 [c0, c1, c2, c3]
        // 上推 k1
        // 左节点(node)变为 [k0] 和 [c0, c1]
        // 右节点(new_node)变为 [k2] 和 [c2, c3]
        pass_key = node->key[1];
        new_node->key[0] = node->key[2];
        new_node->num_keys = 1;
        new_node->child[0] = node->child[2];
        new_node->child[1] = node->child[3];
        if(new_node->child[0]) new_node->child[0]->parent = new_node;
        if(new_node->child[1]) new_node->child[1]->parent = new_node;

        node->key[1] = -1;
        node->key[2] = -1;
        node->num_keys = 1;
        new_node->child[0] = node->child[2];
        new_node->child[1] = node->child[3];
        node->child[2] = NULL;
        node->child[3] = NULL;
    }

    if(parent == NULL){
        Node* new_root = createNode(false, NULL);
        new_root->key[0] = pass_key;
        new_root->num_keys = 1;
        new_root->child[0] = node;
        new_root->child[1] = new_node;
        node->parent = new_root;
        new_node->parent = new_root;
        return new_root; //根节点改变了
    }

    // 如果父节点存在，将上推的键和新节点插入父节点
    // 1. 找到 node 在 parent->child[] 中的位置
    int child_idx = 0;
    while(child_idx <= parent->num_keys && parent->child[child_idx] != node) {
        child_idx++;
    }

    // 2. 为新的键和子节点指针腾出空间
    for (int i = parent->num_keys; i > child_idx; i--) {
        parent->key[i] = parent->key[i - 1];
        parent->child[i + 1] = parent->child[i];
    }

    // 3. 插入上推的键和新的右子节点
    parent->key[child_idx] = pass_key;
    parent->child[child_idx + 1] = new_node;
    parent->num_keys++;

    // 4. 检查父节点是否也需要分裂
    if (parent->num_keys > 2) {
        return splitNode(false, parent);
    }

    return NULL; // 父节点未分裂，根节点没有改变
}

Node* insert(int key, Node* root){
    if(root == NULL){
        root = createNode(true, NULL);
        root->key[0] = key;
        root->num_keys = 1;
        return root;
    }else{
        Node* leaf = findLeaf(key, root);
        for(int i=0; i<leaf->num_keys; i++){
            if(leaf->key[i] == key){
                printf("Key %d is duplicated\n", key);
                return root;
            }
        }
        Node* new_root = insertIntoLeaf(leaf, key);
        if(new_root != NULL) return new_root; //根节点改变了
        else return root; //根节点没有改变
    }
}

//层序遍历
void traverse(Node* root){
    if (root == NULL) return;
    Node* queue[10001];
    int front = 0, rear = 0;
    queue[rear++] = root;
    int flag = 0;

    while (front < rear) {
        if(flag>0) printf("\n");
        int level_size = rear - front;
        for (int i = 0; i < level_size; i++) {
            Node* current = queue[front++];
            printf("[");
            for (int j = 0; j < current->num_keys; j++) {
                if(current->key[j] == -1) break;
                printf("%d", current->key[j]);
                if (j < current->num_keys - 1) printf(",");
            }
            printf("]");

            if (!current->is_leaf) {
                for (int j = 0; j <= current->num_keys; j++) {
                    if (current->child[j]) queue[rear++] = current->child[j];
                }
            }
        }
        flag++;
        //printf("\n");
    }
}

int main(){
    int N;
    scanf("%d", &N);
    Node* root = NULL;
    for(int i=0; i<N; i++){
        int key;
        scanf("%d", &key);
        root = insert(key, root);
    }
    traverse(root);
    return 0;
}