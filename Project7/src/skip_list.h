#ifndef SKIP_LIST_H
#define SKIP_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LEVEL 24 //the maximal input scale is set as 10^6, so 24 levels are enough
#define SKIPLIST_P 0.5f //probability factor to lift as 0.5

typedef struct skip_list_node {
    int data; // the value of the node
    int maxLevel; // the level of the node
    struct skip_list_node *forward[MAX_LEVEL]; // the next node pointers for each level
} skip_list_node;

typedef struct skip_list {
    int levelCount; // the  highest level of the skip list
    skip_list_node *head; // pointer to the head node of the skip list
} skip_list;

/* related functions */
skip_list *skip_list_create();
void skip_list_free(skip_list *list);
skip_list_node *skip_list_find(skip_list *list, int value);
void skip_list_insert(skip_list *list, int value);
void skip_list_delete(skip_list *list, int value);
static int random_level();

#endif
