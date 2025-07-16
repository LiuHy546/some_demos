add_custom_command(
    OUTPUT moduleC2b.o
    COMMAND ${CMAKE_C_COMPILER} -c moduleC2b.c -o moduleC2b.o -I${CMAKE_SOURCE_DIR}/src -I../../../../ -I../../../moduleA/moduleA2/moduleA2b -I../../../moduleB/moduleB2/moduleB2b
    DEPENDS moduleC2b.c
    COMMENT "Compiling moduleC2b.c to moduleC2b.o"
)
add_custom_target(moduleC2b_obj ALL DEPENDS moduleC2b.o) 