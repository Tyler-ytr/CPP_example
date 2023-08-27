cmake_minimum_required(VERSION 3.0)

project(YourProjectName)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
  include
  example
)

# 添加可执行文件
add_executable(client example/common.cpp example/client.cpp )
add_executable(server example/common.cpp example/server.cpp example/inetaddress.cpp example/socket.cpp example/epoll.cpp)