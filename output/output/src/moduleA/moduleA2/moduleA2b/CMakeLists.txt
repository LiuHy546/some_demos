add_custom_command(
    OUTPUT moduleA2b.o
    COMMAND ${CMAKE_C_COMPILER} -c moduleA2b.c -o moduleA2b.o -I${CMAKE_SOURCE_DIR}/src -I../../../../ -I../moduleA2a -I../../../moduleB/moduleB1/moduleB1b
    DEPENDS moduleA2b.c
    COMMENT "Compiling moduleA2b.c to moduleA2b.o"
)
add_custom_target(moduleA2b_obj ALL DEPENDS moduleA2b.o) 