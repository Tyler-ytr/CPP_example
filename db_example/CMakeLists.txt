#版本号
cmake_minimum_required(VERSION 3.8)

#项目名称
project(db_example)

#设置编译器标准为C++14
SET(CMAKE_CXX_STANDARD 14)

#设置头文件目录
include_directories(${PROJECT_SOURCE_DIR}/include)

#设置库文件目录
add_subdirectory(${PROJECT_SOURCE_DIR}/src)

#设置测试目录
add_subdirectory(${PROJECT_SOURCE_DIR}/tests)


