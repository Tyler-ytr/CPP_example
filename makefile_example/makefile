# 指定编译器
CXX = g++

# 指定编译选项
CXXFLAGS = -std=c++11 -Wall

# 指定可执行文件名称
TARGET = program

# 指定源文件名称
SOURCE = main.cpp

# 指定测试样例文件夹名称
TEST_DIR = test

# 指定测试样例文件名称
TEST_INPUT = input.txt
TEST_OUTPUT = output.txt

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) $< -o $@

test:
	./$(TARGET) < $(TEST_DIR)/$(TEST_INPUT) > $(TEST_DIR)/$(TEST_OUTPUT)

clean:
	rm -f $(TARGET)

.PHONY: all test clean
