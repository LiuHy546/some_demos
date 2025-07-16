cmake_minimum_required(VERSION 3.10)
project(ComplexCProject C)

# 递归查找所有C文件
file(GLOB_RECURSE ALL_C_FILES
    "src/**/*.c"
)

# 为每个C文件生成对应的.o目标
foreach(C_FILE ${ALL_C_FILES})
    get_filename_component(C_DIR ${C_FILE} DIRECTORY)
    get_filename_component(C_NAME_WE ${C_FILE} NAME_WE)
    set(OBJ_FILE ${C_DIR}/${C_NAME_WE}.o)
    add_custom_command(
        OUTPUT ${OBJ_FILE}
        COMMAND ${CMAKE_C_COMPILER} -c ${C_FILE} -o ${OBJ_FILE} -I${C_DIR} -I${CMAKE_SOURCE_DIR}/src
        DEPENDS ${C_FILE}
        COMMENT "Compiling ${C_FILE} to ${OBJ_FILE}"
    )
    add_custom_target(${C_NAME_WE}_obj ALL DEPENDS ${OBJ_FILE})
endforeach() 