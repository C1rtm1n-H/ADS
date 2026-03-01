# Makefile for Linux

CC = gcc
# Add -D_POSIX_C_SOURCE=200809L to enable strdup and other POSIX functions
# Add -Werror=implicit-function-declaration to catch missing prototypes and prevent pointer truncation
CFLAGS = -Wall -std=c99 -O2 -g -D_POSIX_C_SOURCE=200809L -Werror=implicit-function-declaration -I. -I./src
LDFLAGS = -lm
PTHREAD_FLAGS = -pthread

# 目标文件
SERIAL_TARGET = serial_wc
PARALLEL_TARGET = parallel_wc
GENERATE_TARGET = generate_test_data

# 源文件
SERIAL_SRC = src/serial_wordcount.c src/utils/hash_table.c src/utils/sort.c src/utils/file_utils.c
PARALLEL_SRC = src/parallel_wordcount.c src/mapreduce.c src/utils/hash_table.c src/utils/sort.c src/utils/file_utils.c
GENERATE_SRC = src/testcase/generate_test_data.c

# 默认目标
all: $(SERIAL_TARGET) $(PARALLEL_TARGET) $(GENERATE_TARGET)

# 串行版本 (依赖 makefile 以确保 flag 变更后重新编译)
$(SERIAL_TARGET): $(SERIAL_SRC) makefile
	$(CC) $(CFLAGS) -o $@ $(SERIAL_SRC) $(LDFLAGS)

# 并行版本
$(PARALLEL_TARGET): $(PARALLEL_SRC) makefile
	$(CC) $(CFLAGS) $(PTHREAD_FLAGS) -o $@ $(PARALLEL_SRC) $(LDFLAGS)

# 测试数据生成器
$(GENERATE_TARGET): $(GENERATE_SRC) makefile
	$(CC) $(CFLAGS) -o $@ $(GENERATE_SRC) $(LDFLAGS)

# 创建测试目录
testdir:
	mkdir -p test

# 生成测试数据
testdata: $(GENERATE_TARGET) testdir
	@echo "Generating test data..."
	./$(GENERATE_TARGET) test/test_data_10000.txt 10000
	./$(GENERATE_TARGET) test/test_data_50000.txt 50000
	./$(GENERATE_TARGET) test/test_data_100000.txt 100000
	./$(GENERATE_TARGET) test/test_data_200000.txt 200000
	./$(GENERATE_TARGET) test/test_data_500000.txt 500000
	./$(GENERATE_TARGET) test/test_data_1000000.txt 1000000
	./$(GENERATE_TARGET) test/test_data_2000000.txt 2000000
	./$(GENERATE_TARGET) test/test_data_5000000.txt 5000000
	@echo "Test data generated in 'test' directory."

# 运行简单测试
test: all testdata
	@echo "Execute simple testcase with 100,000 words:"
	@echo "1. run serial version..."
	./$(SERIAL_TARGET) test/serial_output_100000.txt test/test_data_100000.txt
	@echo "2. run parallel version..."
	./$(PARALLEL_TARGET) test/parallel_output_100000.txt test/test_data_100000.txt
	@echo "3. verify result consistency..."
	python3 test/verify_results.py test/serial_output_100000.txt test/parallel_output_100000.txt

# 运行性能基准测试
benchmark: all testdata
	python3 test/benchmark.py

# 绘制性能图表
plot: benchmark
	python3 test/plot_performance.py

# 清理
clean:
	rm -f $(SERIAL_TARGET) $(PARALLEL_TARGET) $(GENERATE_TARGET)
	rm -f *.o
	rm -f test/*.txt test/*.csv test/*.png

# 完全清理
cleanall: clean
	rm -f serial_wc parallel_wc generate_test_data 2>nul

.PHONY: all testdir testdata test benchmark plot clean cleanall