# CMake entry point
# 可以批量编译example中的cpp文件；他们的头文件位于include文件夹中
cmake_minimum_required (VERSION 3.5)
project (cpp_server)
 
## Add headers files
include_directories (
	include/
)
 
# set(HEADERS include/SafeQueue.h include/ThreadPool.h)
 
# set(SOURCES example/main.cpp)

SET(CMAKE_CXX_FLAGS -pthread)
 
# add_compile_options(
# 	-std=c++17
# #        -D_DEBUG
# #        -g
# ) 
set(CMAKE_CXX_STANDARD 17)

# 设置头文件路径
include_directories(include)
# 设置要编译的文件列表
file(GLOB EXAMPLE_SOURCES "example/*.cpp")

# 编译所有的cpp文件
foreach(example_source ${EXAMPLE_SOURCES})
    # 获取文件名，去除.cpp后缀
    get_filename_component(example_name ${example_source} NAME_WE)
    # 生成可执行文件
    add_executable(${example_name} ${example_source})
endforeach(example_source)
# # main.cpp
# add_executable(main ${HEADERS} ${SOURCES})
# target_link_libraries(main)