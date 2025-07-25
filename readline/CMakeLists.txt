cmake_minimum_required(VERSION 3.10)
project(readline_app)

# 设置C标准
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 启用位置无关代码（动态库必需）
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# 1. 编译readline.c和test.c为动态库libreadline_test.so
add_library(readline_test SHARED
    readline.c
    test.c
)

# 设置动态库输出目录
set_target_properties(readline_test PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

# 2. 链接生成可执行文件（依赖libreadline_test.so和libmain.so）
add_executable(main_app main.c)  # main_dummy.c可以是空文件，因为main在libmain.so中

# 指定库搜索路径（当前目录和构建输出的lib目录）
target_link_directories(main_app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}/lib
)

# 链接动态库
target_link_libraries(main_app PRIVATE
    -Wl,--no-as-needed  # 强制加载后续库，即使无符号引用
    readline_test
    -Wl,--as-needed    # 恢复默认行为
)

# 设置可执行文件输出目录
set_target_properties(main_app PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
