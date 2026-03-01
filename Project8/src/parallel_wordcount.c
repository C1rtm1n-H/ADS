#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mapreduce.h"

#define DEFAULT_NUM_MAPPERS 4
#define DEFAULT_NUM_REDUCERS 4

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <output_file> <input_file1> [input_file2 ...]\n", argv[0]);
        return 1;
    }
    
    int num_mappers = DEFAULT_NUM_MAPPERS;
    int num_reducers = DEFAULT_NUM_REDUCERS;
    
    char *env_mappers = getenv("NUM_MAPPERS");
    if (env_mappers) num_mappers = atoi(env_mappers);
    
    char *env_reducers = getenv("NUM_REDUCERS");
    if (env_reducers) num_reducers = atoi(env_reducers);

    // 准备配置
    struct MapReduceConfig config = {
        .num_mappers = num_mappers,
        .num_reducers = num_reducers,
        .input_files = &argv[2],
        .num_input_files = argc - 2,
        .output_file = argv[1]
    };
    
    // 创建MapReduce上下文
    struct MapReduceContext *context = mapreduce_init(config);
    if (!context) {
        fprintf(stderr, "Error initializing MapReduce context\n");
        return 1;
    }
    
    // 运行MapReduce
    int result = mapreduce_run(context);
    
    if (result == 0) {
        printf("MapReduce job completed successfully\n");
        printf("Output written to %s\n", argv[1]);
    } else {
        fprintf(stderr, "MapReduce job failed with error %d\n", result);
    }
    
    // 清理
    mapreduce_cleanup(context);
    
    return result;
}