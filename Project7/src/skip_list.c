#include "skip_list.h"

/* Generate a random level for a new node */
static int random_level() {
    int level = 1; //start as level 1,with a probability of 0.5 to rise to next level
    while ((rand() / (RAND_MAX + 1.0)) < SKIPLIST_P && level < MAX_LEVEL) { // not over the max_level
        level++;
    }
    return level;
}

/* Create a new skip list */
skip_list *skip_list_create() {
    skip_list *list = (skip_list *)malloc(sizeof(skip_list));
    if (!list) return NULL;
    // the list starts with level 1 and a head node
    list->levelCount = 1;
    list->head = (skip_list_node *)malloc(sizeof(skip_list_node));
    if (!list->head) {
        free(list);
        return NULL;
    }
    
    list->head->data = -1; // head node store a meaningless value,which means it does not store anything
    list->head->maxLevel = 0;
    for (int i = 0; i < MAX_LEVEL; i++) {
        list->head->forward[i] = NULL;
    }
    
    srand((unsigned)time(NULL)); // generate a seed
    return list;
}

/* free the skip list and free all memory */
void skip_list_free(skip_list *list) {
    if (!list) return;
    
    skip_list_node *current = list->head->forward[0];
    while (current) {
        skip_list_node *next = current->forward[0];
        free(current);
        current = next;
    }
    
    free(list->head);
    free(list);
}

/* Find a node with the given value */
skip_list_node *skip_list_find(skip_list *list, int value) {
    if (!list || !list->head) return NULL;
    // start from the head node and search for the value
    skip_list_node *p = list->head;
    for (int i = list->levelCount - 1; i >= 0; --i) {
        // move forward if the next node's value is less than the value
        while (p->forward[i] != NULL && p->forward[i]->data < value) {
            p = p->forward[i];
        }
    }
    // ensure the value is in the list
    if (p->forward[0] != NULL && p->forward[0]->data == value) {
        return p->forward[0];
    } else {
        return NULL;
    }
}

/* Insert a value into the skip list */
void skip_list_insert(skip_list *list, int value) {
    if (!list) return;
    
    skip_list_node *update[MAX_LEVEL];
    skip_list_node *p = list->head;

    //search from the top level of the list down to 0
    for (int i = list->levelCount - 1; i >= 0; --i) {
        while (p->forward[i] != NULL && p->forward[i]->data < value) {
            p = p->forward[i];
        }
        update[i] = p; 
    }
    
    //if the node exists, don't insert
    if (p->forward[0] != NULL && p->forward[0]->data == value) {
        return;
    }
    
    int level = random_level();

    // If the new level is higher than current levelCount, initialize update array for new levels
    if (level > list->levelCount) {
        for (int i = list->levelCount; i < level; i++) {
            update[i] = list->head;
        }
        list->levelCount = level;
    }
    
    skip_list_node *newNode = (skip_list_node *)malloc(sizeof(skip_list_node));
    if (!newNode) return;
    // create the new node
    newNode->data = value;
    newNode->maxLevel = level;
    for (int i = 0; i < MAX_LEVEL; i++) {
        newNode->forward[i] = NULL;
    }
    
    // insert the new node at every level
    for (int i = 0; i < level; ++i) {
        newNode->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = newNode;
    }
}

/* Delete a value from the skip list */
void skip_list_delete(skip_list *list, int value) {
    if (!list) return;
    
    skip_list_node *update[MAX_LEVEL];
    skip_list_node *p = list->head;
    // find the biggest node smaller than value at each level
    for (int i = list->levelCount - 1; i >= 0; --i) {
        while (p->forward[i] != NULL && p->forward[i]->data < value) {
            p = p->forward[i];
        }
        update[i] = p;
    }
    // ensure the value is in the list
    if (p->forward[0] != NULL && p->forward[0]->data == value) {
        skip_list_node *toDelete = p->forward[0];
        // remove the node from each level
        for (int i = list->levelCount - 1; i >= 0; --i) {
            if (update[i]->forward[i] != NULL && update[i]->forward[i]->data == value) {
                update[i]->forward[i] = update[i]->forward[i]->forward[i];
            }
        }
        
        free(toDelete);
        
        // update the levelCount of the skip list
        while (list->levelCount > 1 && list->head->forward[list->levelCount - 1] == NULL) {
            list->levelCount--;
        }
    }
}
