#ifndef DIF_FUC_H
#define DIF_FUC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define maxNum 10001

// the rectangle structure
typedef struct item {
    int width;      // the width of the rectangle
    int height;     // the height of the rectangle
    int x;          // the x point of the left-bottom
    int y;          // the y point of the left-bottom
} Item;

void sortByHeight(Item rect[], int n);
int FFDH(int W, int n, Item rect[]);
int NFDH(int W, int n, Item rect[]);
int BFDH(int W, int n, Item rect[]);
int BL(int W, int n, Item rect[]);
int BL_Online(int W, int n, Item rect[]);
void generateUniform(Item rect[], int n, int minW, int maxW, int minH, int maxH);
void generateNormal(Item rect[], int n, int meanW, int stdW, int meanH, int stdH);
#endif