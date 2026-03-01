#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

#define ORDER 3
#define MAX_KEYS ORDER
#define MAX_CHILDREN ORDER

//define the structure of node
typedef struct Node {
    int key[MAX_KEYS];
    struct Node* child[MAX_CHILDREN];
    struct Node* parent;
    struct Node* next_leaf;
    int num_keys;
    bool is_leaf;
} Node;

Node* create_node(bool is_leaf, Node* parent);
Node* insert(Node* root, int key);
void traverse(Node* root);
static Node* find_leaf(Node* root, int key);
static Node* insert_into_leaf(Node* leaf, int key);
static Node* insert_into_parent(Node* left, int key, Node* right);

Node* create_node(bool is_leaf, Node* parent){
    Node* new_node = (Node*)malloc(sizeof(Node));
    if(!new_node) {
        perror("Malloc failed");
        exit(EXIT_FAILURE);
    }
    new_node->is_leaf = is_leaf;
    new_node->num_keys = 0;
    new_node->parent = parent;
    new_node->next_leaf = NULL;
    for(int i=0; i<MAX_KEYS; i++) new_node->key[i] = 0;
    for(int i=0; i<MAX_CHILDREN; i++) new_node->child[i] = NULL;
    return new_node;
}

static Node* find_leaf(Node* root, int key){
    if (root == NULL) return NULL;
    Node* current = root;
    while(current->is_leaf == false){
        int i=0;
        while(i < current->num_keys && key >= current->key[i]) {
            i++;
        }
        current = current->child[i];
    }
    return current;
}

static Node* insert_into_parent(Node* left, int key, Node* right) {
    Node* parent = left->parent;
    if (parent == NULL) { // 如果没有父节点，创建一个新的根
        Node* new_root = create_node(false, NULL);
        new_root->key[0] = key;
        new_root->child[0] = left;
        new_root->child[1] = right;
        new_root->num_keys = 1;
        left->parent = new_root;
        right->parent = new_root;
        return new_root;
    }

    // 如果父节点有空间
    if (parent->num_keys < MAX_KEYS) {
        int i=0;
        while(i < parent->num_keys && parent->key[i] < key) {
            i++;
        }
        for(int j = parent->num_keys; j > i; j--) {
            parent->key[j] = parent->key[j - 1];
            parent->child[j + 1] = parent->child[j];
        }
        parent->key[i] = key;
        parent->child[i + 1] = right;
        parent->num_keys++;
        return NULL; // 根节点没有改变
    }

    // 如果父节点也满了，分裂
    int temp_keys[MAX_KEYS + 1];
    Node* temp_children[MAX_CHILDREN + 1];

    int insertion_index = 0;
    while (insertion_index < MAX_KEYS && parent->key[insertion_index] < key) {
        insertion_index++;
    }

    for (int i = 0; i < insertion_index; i++) {
        temp_keys[i] = parent->key[i];
        temp_children[i] = parent->child[i];
    }
    temp_children[insertion_index] = parent->child[insertion_index];

    temp_keys[insertion_index] = key;
    temp_children[insertion_index + 1] = right;

    for (int i = insertion_index; i < MAX_KEYS; i++) {
        temp_keys[i + 1] = parent->key[i];
        temp_children[i + 2] = parent->child[i + 1];
    }

    int median_idx = (MAX_KEYS + 1) / 2;
    int median_key = temp_keys[median_idx];

    parent->num_keys = median_idx;
    for(int j=0; j<median_idx; j++) {
        parent->key[j] = temp_keys[j];
        parent->child[j] = temp_children[j];
    }
    parent->child[median_idx] = temp_children[median_idx];
    for(int j = median_idx + 1; j < MAX_CHILDREN; j++) {
        parent->child[j] = NULL;
    }

    Node* new_node = create_node(false, parent->parent);
    new_node->num_keys = MAX_KEYS - median_idx;
    for(int j=0; j<new_node->num_keys; j++) {
        new_node->key[j] = temp_keys[median_idx + 1 + j];
        new_node->child[j] = temp_children[median_idx + 1 + j];
        if(new_node->child[j]) new_node->child[j]->parent = new_node;
    }
    new_node->child[new_node->num_keys] = temp_children[median_idx + 1 + new_node->num_keys];
    if(new_node->child[new_node->num_keys]) new_node->child[new_node->num_keys]->parent = new_node;

    return insert_into_parent(parent, median_key, new_node);
}

static Node* insert_into_leaf(Node* leaf, int key) {
    // 如果叶子有空间
    if (leaf->num_keys < MAX_KEYS) {
        int i = 0;
        while (i < leaf->num_keys && leaf->key[i] < key) {
            i++;
        }
        for (int j = leaf->num_keys; j > i; j--) {
            leaf->key[j] = leaf->key[j - 1];
        }
        leaf->key[i] = key;
        leaf->num_keys++;
        return NULL; // 根节点没有改变
    }

    // 如果叶子满了，分裂
    Node* new_leaf = create_node(true, leaf->parent);
    int temp_keys[MAX_KEYS + 1];
    int i = 0;
    while (i < MAX_KEYS && leaf->key[i] < key) {
        temp_keys[i] = leaf->key[i];
        i++;
    }
    temp_keys[i] = key;
    for (int j = i; j < MAX_KEYS; j++) {
        temp_keys[j + 1] = leaf->key[j];
    }

    int split_point = (MAX_KEYS + 1) / 2;
    leaf->num_keys = split_point;
    new_leaf->num_keys = (MAX_KEYS + 1) - split_point;

    for (i = 0; i < leaf->num_keys; i++) {
        leaf->key[i] = temp_keys[i];
    }
    for (i = 0; i < new_leaf->num_keys; i++) {
        new_leaf->key[i] = temp_keys[split_point + i];
    }

    new_leaf->next_leaf = leaf->next_leaf;
    leaf->next_leaf = new_leaf;

    return insert_into_parent(leaf, new_leaf->key[0], new_leaf);
}

Node* insert(Node* root, int key) {
    if (root == NULL) {
        root = create_node(true, NULL);
        root->key[0] = key;
        root->num_keys = 1;
        return root;
    }

    Node* leaf = find_leaf(root, key);

    for(int i=0; i<leaf->num_keys; i++){
        if(leaf->key[i] == key){
            printf("Key %d is duplicated\n", key);
            return root;
        }
    }

    Node* new_root = insert_into_leaf(leaf, key);
    return new_root ? new_root : root;
}

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
        root = insert(root, key);
    }
    traverse(root);
    return 0;
}