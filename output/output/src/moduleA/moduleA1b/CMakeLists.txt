add_custom_command(
    OUTPUT moduleA1b.o
    COMMAND ${CMAKE_C_COMPILER} -c moduleA1b.c -o moduleA1b.o -I${CMAKE_SOURCE_DIR}/src -I../../ -I../../moduleB/moduleB1/moduleB1b -I../../moduleC/moduleC1/moduleC1b
    DEPENDS moduleA1b.c
    COMMENT "Compiling moduleA1b.c to moduleA1b.o"
)
add_custom_target(moduleA1b_obj ALL DEPENDS moduleA1b.o) 