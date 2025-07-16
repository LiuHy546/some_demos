cmake_minimum_required(VERSION 3.10)
project(test_proj C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(test_main main.c)

# 链接 output 目录下的 mylib.a
set(MYLIB_PATH "${CMAKE_SOURCE_DIR}/../mylib.a")
target_link_libraries(test_main PRIVATE ${MYLIB_PATH})

# 包含 output 目录下的头文件
include_directories("${CMAKE_SOURCE_DIR}/../src") 