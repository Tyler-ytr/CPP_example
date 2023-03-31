#版本号
cmake_minimum_required(VERSION 3.8)

add_library(BPTREE BPtree.cpp)
add_library(db_example db_example.cpp BPtree.cpp command_parser.cpp)


