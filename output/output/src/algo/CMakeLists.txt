add_custom_command(
    OUTPUT sort.o
    COMMAND ${CMAKE_C_COMPILER} -c sort.c -o sort.o -I${CMAKE_SOURCE_DIR}/src
    DEPENDS sort.c
    COMMENT "Compiling sort.c to sort.o"
)
add_custom_target(sort_obj ALL DEPENDS sort.o) 