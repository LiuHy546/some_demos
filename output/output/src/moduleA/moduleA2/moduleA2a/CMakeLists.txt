add_custom_command(
    OUTPUT moduleA2a.o
    COMMAND ${CMAKE_C_COMPILER} -c moduleA2a.c -o moduleA2a.o -I${CMAKE_SOURCE_DIR}/src -I../../../../ -I../../moduleA1/moduleA1a -I../../../moduleB/moduleB1/moduleB1a -I../../../moduleC/moduleC2/moduleC2a
    DEPENDS moduleA2a.c
    COMMENT "Compiling moduleA2a.c to moduleA2a.o"
)
add_custom_target(moduleA2a_obj ALL DEPENDS moduleA2a.o) 