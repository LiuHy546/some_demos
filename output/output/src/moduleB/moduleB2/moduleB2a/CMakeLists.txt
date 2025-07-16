add_custom_command(
    OUTPUT moduleB2a.o
    COMMAND ${CMAKE_C_COMPILER} -c moduleB2a.c -o moduleB2a.o -I${CMAKE_SOURCE_DIR}/src -I../../../../ -I../../../moduleA/moduleA2/moduleA2a -I../../../moduleC/moduleC2/moduleC2a
    DEPENDS moduleB2a.c
    COMMENT "Compiling moduleB2a.c to moduleB2a.o"
)
add_custom_target(moduleB2a_obj ALL DEPENDS moduleB2a.o) 