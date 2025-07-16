add_custom_command(
    OUTPUT moduleA1a.o
    COMMAND ${CMAKE_C_COMPILER} -c moduleA1a.c -o moduleA1a.o -I${CMAKE_SOURCE_DIR}/src -I../../../../ -I../../moduleA1b -I../../../moduleB/moduleB1/moduleB1a -I../../../moduleC/moduleC1/moduleC1a
    DEPENDS moduleA1a.c
    COMMENT "Compiling moduleA1a.c to moduleA1a.o"
)
add_custom_target(moduleA1a_obj ALL DEPENDS moduleA1a.o) 