add_custom_command(
    OUTPUT moduleB1b.o
    COMMAND ${CMAKE_C_COMPILER} -c moduleB1b.c -o moduleB1b.o -I${CMAKE_SOURCE_DIR}/src -I../../../../ -I../../../moduleC/moduleC1/moduleC1a
    DEPENDS moduleB1b.c
    COMMENT "Compiling moduleB1b.c to moduleB1b.o"
)
add_custom_target(moduleB1b_obj ALL DEPENDS moduleB1b.o) 