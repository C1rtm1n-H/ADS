
#include "dif_fuc.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_REPETITION 100   
#define CONTAINER_W 100          
//choose different repeat time for different n and algorithms
//for BL algorithm,if n >= 1000,repeat 1 time;for other algorithm,if n==10000,repeat 10 times
#define GET_REPETITION(n, k) \
    ((n >= 1000 && (k == 3 || k == 4)) ? 3 : \
     (n == 10000) ? 10 : DEFAULT_REPETITION)

//fuction pointer,to call the algorithms sequentially
typedef int (*PackingFunc)(int W, int n, Item rect[]);
//store the results of specific n,testmode,algorithm
void store(const char* algorithm, const char* testMode, int n, double runtime, double fillRatio, int height, const char* filename) {
    FILE* f = fopen(filename, "a");
    if (f == NULL) {
        perror("can't open the file");
        return;
    }
    fprintf(f, "%s,%s,%d,%.4f,%.4f,%d\n", algorithm, testMode, n, runtime, fillRatio, height);
    fclose(f);
}

//initialize the csv file
void init(const char* filename) {
    FILE* f = fopen(filename, "w");
    if (f == NULL) {
        perror("can't open the file");
        return;
    }
    fprintf(f, "Algorithm,TestMode,N,Runtime(ms),FillRatio,Height\n");
    fclose(f);
}

//store the data to corresponding csv file
void exportVisualData(const char* algorithm, const char* testMode,int W, int H, int n, double fillRatio, Item rect[], const char* filename) {
    int i;
    FILE* f = fopen(filename, "w");
    if (f == NULL) {
        perror("can't open the file");
        return;
    }
    fprintf(f, "%s,%s,%d,%d,%d,%.4f\n", algorithm, testMode, W, H, n, fillRatio);
    for (i = 0; i < n; i++) {
        fprintf(f, "%d,%d,%d,%d,%d\n", i, rect[i].x, rect[i].y, rect[i].width, rect[i].height);
    }
    fclose(f);
    printf("  -> Exported visualization data to %s\n", filename);
}

//copy the rectangle array,not to override the primitive array
void copyRectangles(Item src[], Item dst[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        dst[i] = src[i];
    }
}

//different testmodes
void generateTestData(Item rect[], int n, int mode, int maxSize) {
    int i;
    switch (mode) {
        //1. identical rectangles
        case 0:  
            for (i = 0; i < n; i++) {
                rect[i].width = maxSize / 2;   
                rect[i].height = maxSize / 2; 
                rect[i].x = 0;
                rect[i].y = 0;
            }
            break;
        //2. the width & height of the rectangles obey U(0,maxSize)
        case 1: 
            generateUniform(rect, n, 1, maxSize, 1, maxSize);
            break;
        //3. the width & height of the rectangles obey N(maxSize/2,(maxSize/3)^2),the standard deviation is high
        case 2:
            generateNormal(rect, n, maxSize / 2, maxSize / 3, maxSize / 2, maxSize / 3);
            //restrict the width & height of the rectangles from 1 to CONTAINER_W
            for (i = 0; i < n; i++) {
                if (rect[i].width > CONTAINER_W) rect[i].width = CONTAINER_W;
                if (rect[i].height > CONTAINER_W) rect[i].height = CONTAINER_W;
                if (rect[i].width < 1) rect[i].width = 1;
                if (rect[i].height < 1) rect[i].height = 1;
            }
            break;
    }
}

int main() {
    //5 different algorithms & 3 different testmodes
    const char* Algorithm_Name[5] = {"NFDH", "FFDH", "BFDH", "BL", "BL_Online"};
    PackingFunc algorithms[5] = {NFDH, FFDH, BFDH, BL, BL_Online};
    const char* Test_Mode[3] = {"Identical Rectangles","Uniform Distribution","High Std Normal Distribution"};
    
    //the number of the rectangles(100-10000), denser points for complexity analysis
    int test_n[19] = {100, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 
                      1500, 2000, 2500, 3000, 4000, 5000, 6000, 8000, 10000};
    int num_tests = 19;  //count of n values
    
    //a fixed argument to help generate test rectangles
    int maxSize = 30;
    Item rect[maxNum];    
    Item temp[maxNum];    
    int i, j, k, r;
    clock_t start, end;
    srand((unsigned int)time(NULL));
    init("result.csv");
    
    printf("========================================\n");
    printf("Texture Packing Algorithm Performance Test\n");
    printf("Container Width: %d\n", CONTAINER_W);
    printf("========================================\n\n");
    for (i = 0; i < num_tests; i++) {
        int n = test_n[i];
        printf("Testing n = %d ...\n", n);
        
        for (j = 0; j < 3; j++) { 
            generateTestData(rect, n, j, maxSize);
            for (k = 0; k < 5; k++) {
                int repetition = GET_REPETITION(n, k); 
                double totalTime = 0;
                double totalRatio = 0;
                int totalHeight = 0;
                start = clock();
                for (r = 0; r < repetition; r++) {
                    copyRectangles(rect, temp, n);
                    int height = algorithms[k](CONTAINER_W, n, temp);
                    long long totalArea = 0;
                    for (int l = 0; l < n; l++) {
                        totalArea += (long long)temp[l].width * temp[l].height;
                    }
                    double ratio = (double)totalArea/(long long)(CONTAINER_W * height);
                    totalRatio += ratio;
                    totalHeight += height;
                }
                end = clock();
                //calculate the avg values
                double avgTime = ((double)(end - start)) / CLOCKS_PER_SEC / repetition * 1000;
                double avgRatio = totalRatio / repetition;
                int avgHeight = totalHeight / repetition;
                //store in result.csv
                store(Algorithm_Name[k], Test_Mode[j], n, avgTime, avgRatio, avgHeight, "result.csv");
                printf("  %s + %s: Time=%.4fms, Ratio=%.2f%%, Height=%d\n",Algorithm_Name[k], Test_Mode[j], avgTime, avgRatio * 100, avgHeight);
                //when n == 100, we visualize the texture packing
                if (n == 100) {
                    char visualFile[128];
                    sprintf(visualFile, "visual_%s_mode%d.csv", Algorithm_Name[k], j);
                    copyRectangles(rect, temp, n);
                    int height = algorithms[k](CONTAINER_W, n, temp);
                    long long totalArea = 0;
                    for (int l = 0; l < n; l++) {
                        totalArea += (long long)temp[l].width * temp[l].height;
                    }
                    double ratio = (double)totalArea/(long long)(CONTAINER_W * height);
                    exportVisualData(Algorithm_Name[k], Test_Mode[j],CONTAINER_W, height, n, ratio, temp, visualFile);
                }
            }
        }
        printf("\n");
    }
    printf("========================================\n");
    printf("Test completed! Results saved to result.csv\n");
    return 0;
}

