#include "dif_fuc.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
//compare 2 rectangle's height
static int compareByHeight(const void* a, const void* b) {
    Item* r1 = (Item*)a;
    Item* r2 = (Item*)b;
    if (r2->height != r1->height) {
        return (r2->height > r1->height) ? 1 : -1;
    }
    //if they have the same height,choose the wider one
    return (r2->width > r1->width) ? 1 : -1;
}
//quick sort by the height
void sortByHeight(Item rect[], int n) {
    qsort(rect, n, sizeof(Item), compareByHeight);
}

/*
 
 * NFDH - Next Fit Decreasing Height 
 Sort by decreasing height; pack rectangles sequentially into the current row, start a new row when it no longer fits.
 * @param W: Container width
 * @param n: number of rectangles
 * @param rect: rectangle array
 * @return: final height
 */
int NFDH(int W, int n, Item rect[]) {
    int i;
    int curWidth = 0;        //the current width of the current level
    int curLevelY = 0;       //the bottom of the current level
    int curLevelH = 0;       //the height of the current level       
    sortByHeight(rect, n);
    for (i = 0; i < n; i++) {
        //check if the current level can put the rectangle
        if (curWidth + rect[i].width <= W) {
            //update the location of the rectangle being put,and the width & hight of the current level
            rect[i].x = curWidth;           
            rect[i].y = curLevelY;          
            curWidth += rect[i].width;
            //when we put the first rectangle in a level      
            if (rect[i].height > curLevelH) {
                curLevelH = rect[i].height;
            }
        } 
        //otherwise,open a new level,update the location of the rectangle being put,and the width & hight of the new current level
        else {
            curLevelY = curLevelY + curLevelH; 
            curLevelH = rect[i].height;         
            curWidth = rect[i].width;                       
            rect[i].x = 0;                       
            rect[i].y = curLevelY;              
        }
    }
    //return the final height
    return curLevelY + curLevelH;
}

/*
 * FFDH - First Fit Decreasing Height
 Sort by decreasing height; place each rectangle in the first row where it fits.
 * @param W: Container width
 * @param n: number of rectangles
 * @param rect: rectangle array
 * @return: final height
 */
int FFDH(int W, int n, Item rect[]) {
    int i, j;
    int curHeight = 0;           //the total height of the texture
    int levelW[maxNum];        //the current width of every level(the number of levels <= number of rectangles)
    int levelY[maxNum];          // the bottom of every level
    int levelH[maxNum];          //the height of every level
    int level;                   //the current number of levels
    sortByHeight(rect, n);
//initialization
    level = 0;
    for (i = 0; i < n; i++) {
        levelW[i] = 0;
    }

    
    for (i = 0; i < n; i++) {
        //find the first level the can put the current rectangle
        for (j = 0; j < level; j++) {
            if (levelW[j] + rect[i].width <= W) {
                //if found,update the location of the rectangle being put
                rect[i].x = levelW[j];   
                rect[i].y = levelY[j];     
                levelW[j] += rect[i].width;   
                break;                          
            }
        }
        //put the next rectangle
        if (j < level) {
            continue;
        }
        //if not found, open a new level,update the Y&H&W of it and the total height
        levelY[level] = curHeight;         
        levelH[level] = rect[i].height;         
        curHeight += rect[i].height;            
        levelW[level] = rect[i].width;
        rect[i].x = 0;                          
        rect[i].y = levelY[level];         
        level++;                              
    }

    return curHeight;
}

/*

 * BFDH - Best Fit Decreasing Height
 Sort by decreasing height; choose the row that leaves the least leftover width after placement.
 * @param W: Container width
 * @param n: number of rectangles
 * @param rect: rectangle array
 * @return: final height
 */
int BFDH(int W, int n, Item rect[]) {
    int i, j;
    int curHeight = 0;          
    int levelW[maxNum];       
    int levelY[maxNum];     
    int level;                  
    sortByHeight(rect, n);

    //initialization
    level = 0;
    for (i = 0; i < n; i++) {
        levelW[i] = 0;
    }

    for (i = 0; i < n; i++) {
        int bestLevel = -1;             //the best level to put
        int minRemain = W + 1;          //the minimum remaining width,first set to W+1
        for (j = 0; j < level; j++) {
            int remain = W - levelW[j];   
            
            //find the minimum remain that can put the current rectangle
            if (remain >= rect[i].width && remain < minRemain) {
                bestLevel = j;
                minRemain = remain;
            }
        }
        //if found,update the location of the rectangle being put
        if (bestLevel != -1) {
            rect[i].x = levelW[bestLevel];
            rect[i].y = levelY[bestLevel];
            levelW[bestLevel] += rect[i].width;
        } 
        //if not found,open a new level,update the location of the rectangle being put
        else {
            levelY[level] = curHeight;
            curHeight += rect[i].height;
            levelW[level] = rect[i].width;
            rect[i].x = 0;
            rect[i].y = levelY[level];
            level++;
        }
    }
    return curHeight;
}

/*
 * BL - Bottom-up Left-justified
 Sort by decreasing height,Place rectangles by scanning from bottom-left
 * @param W: Container width
 * @param n: number of rectangles
 * @param rect: rectangle array
 * @return: final height
 */
int BL(int W, int n, Item rect[]) {
    int i, j;
    int maxHeight = 0;  
    sortByHeight(rect, n);
    //initialization
    for (i = 0; i < n; i++) {
        rect[i].x = 0;
        rect[i].y = 0;
    }
    for (i = 0; i < n; i++) {
        int bestX = 0;       //final x value
        int bestY = 1e9;     //final y value,first set as maximum
        int x;              
        //try every possible x
        for (x = 0; x <= W - rect[i].width; x++) {
            //record the lowest putable y of the specific x
            int putableY = 0;
            for (j = 0; j < i; j++) {
                //if the rect[i] and rect[j] cover each other partially 
                int cover = !(x + rect[i].width <= rect[j].x || rect[j].x + rect[j].width <= x);
                if (cover) {
                    //find the biggest putable y
                    int topY = rect[j].y + rect[j].height;
                    if (topY > putableY) {
                        putableY = topY;
                    }
                }
            }
            //use the lowest putableY for different x
            if (putableY < bestY || (putableY == bestY && x < bestX)) {
                bestX = x;
                bestY = putableY;
            }
        }
        //put the rect to the best position
        rect[i].x = bestX;
        rect[i].y = bestY;
        //update the maximum height
        if (rect[i].y + rect[i].height > maxHeight) {
            maxHeight = rect[i].y + rect[i].height;
        }
    }

    return maxHeight;
}

/*
 * BL_Online - Online Bottom-Left
 Place rectangles by scanning from bottom-left, filling gaps greedily without pre-sorting.
 * @param W: Container width
 * @param n: number of rectangles
 * @param rect: rectangle array
 * @return: final height
 */
int BL_Online(int W, int n, Item rect[]) {
    int i, j;
    int maxHeight = 0;  
    //initialization
    for (i = 0; i < n; i++) {
        rect[i].x = 0;
        rect[i].y = 0;
    }
    for (i = 0; i < n; i++) {
        int bestX = 0;       //final x value
        int bestY = 1e9;     //final y value,first set as maximum
        int x;              
        //try every possible x
        for (x = 0; x <= W - rect[i].width; x++) {
            //record the lowest putable y of the specific x
            int putableY = 0;
            for (j = 0; j < i; j++) {
                //if the rect[i] and rect[j] cover each other partially 
                int cover = !(x + rect[i].width <= rect[j].x || rect[j].x + rect[j].width <= x);
                if (cover) {
                    //find the biggest putable y
                    int topY = rect[j].y + rect[j].height;
                    if (topY > putableY) {
                        putableY = topY;
                    }
                }
            }
            //use the lowest putableY for different x
            if (putableY < bestY || (putableY == bestY && x < bestX)) {
                bestX = x;
                bestY = putableY;
            }
        }
        //put the rect to the best position
        rect[i].x = bestX;
        rect[i].y = bestY;
        //update the maximum height
        if (rect[i].y + rect[i].height > maxHeight) {
            maxHeight = rect[i].y + rect[i].height;
        }
    }

    return maxHeight;
}
/*
 * test generate function(uniform)
 * @param rect: the rectangle array
 * @param n: number of rectangles
 * @param minW, maxW: the range of width
 * @param minH, maxH: the range of height
 */
void generateUniform(Item rect[], int n, int minW, int maxW, int minH, int maxH) {
    int i;
    for (i = 0; i < n; i++) {
        rect[i].width = minW + rand() % (maxW - minW + 1);
        rect[i].height = minH + rand() % (maxH - minH + 1);
        rect[i].x = 0;
        rect[i].y = 0;
    }
}
static double randNormal(double mean, double std) {
    double u1 = (rand() + 1.0) / (RAND_MAX + 2.0); 
    double u2 = (rand() + 1.0) / (RAND_MAX + 2.0);
    double z = sqrt(-2.0 * log(u1)) * cos(2.0 * 3.14159265358979 * u2);
    return mean + std * z;
}
/* 
 * test generate function(normal)
 * @param rect: the rectangle array
 * @param n: number of rectangles
 * @param meanW, stdW: the means & std of the width
 * @param meanH, stdH: the means & std of the height
 */
void generateNormal(Item rect[], int n, int meanW, int stdW, int meanH, int stdH) {
    int i;
    for (i = 0; i < n; i++) {
        double w = randNormal((double)meanW, (double)stdW);
        double h = randNormal((double)meanH, (double)stdH);
        rect[i].width = (int)(w + 0.5);
        rect[i].height = (int)(h + 0.5);
        if (rect[i].width < 1) rect[i].width = 1;
        if (rect[i].height < 1) rect[i].height = 1;
        rect[i].x = 0;
        rect[i].y = 0;
    }
}
/* 
 * copy the rectangle array to avoid override the original array
 */
static void copyRect(Item src[], Item dst[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        dst[i] = src[i];
    }
}