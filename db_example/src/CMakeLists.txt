#版本号
cmake_minimum_required(VERSION 3.8)

add_library(BPTREE BPtree.cpp)
add_library(db_example db_example.cpp BPtree.cpp command_parser.cpp table.cpp page.cpp)
add_library(BITMAP bit_map.cpp)
add_library(PAGE page.cpp)
