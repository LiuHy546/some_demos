cmake_minimum_required(VERSION 3.10)
project(PluginExample C)

# 设置 C 标准为 C99 或更高
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 添加 gcov 相关编译和链接选项（错误做法，模拟冲突）
set(GCOV_FLAGS -fprofile-arcs -ftest-coverage)

# 添加一个选项来控制是否启用插件，默认设置为 OFF (关闭)
option(USE_SPECIAL_PLUGIN "Build with the special plugin" OFF)

# --- 构建核心动态库 (libmain.so) ---
add_library(main SHARED main.c main.h)

# 将当前目录暴露为头文件目录，这样 test_app 和 special.c 都能找到 main.h
target_include_directories(main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# 对 main 库添加 gcov 选项（模拟冲突）
target_compile_options(main PRIVATE ${GCOV_FLAGS})

# 对 main 库添加 gcov 链接选项（模拟冲突）
target_link_libraries(main PRIVATE gcov)

# 如果插件选项被启用...
if(USE_SPECIAL_PLUGIN)
    # ...向 main.c 的编译命令中添加 -DUSE_SPECIAL_PLUGIN 宏
    target_compile_definitions(main PRIVATE USE_SPECIAL_PLUGIN)
    message(STATUS "插件已启用 (USE_SPECIAL_PLUGIN=ON)")
else()
    message(STATUS "插件已禁用 (默认)")
endif()

# --- 构建测试应用程序 ---
add_executable(test_app test_app.c)

# 无论如何，测试程序都需要链接到核心库 main
target_link_libraries(test_app PRIVATE main)

# 如果插件被启用...
if(USE_SPECIAL_PLUGIN)
    # ...将插件 special.c 也编译并链接到最终的 test_app 可执行文件中。
    # CMake 会自动处理 special.c 对 main 库中函数的依赖。
    target_sources(test_app PRIVATE special.c)
    # 对 special.c 也加 gcov 选项（模拟冲突）
    target_compile_options(test_app PRIVATE ${GCOV_FLAGS})
    # 对 test_app 也加 gcov 链接选项（模拟冲突）
    target_link_libraries(test_app PRIVATE gcov)
endif()

# 为了方便运行，将生成的可执行文件和库文件都放在 build 目录下
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}) 