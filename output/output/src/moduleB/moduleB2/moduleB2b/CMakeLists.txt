add_custom_command(
    OUTPUT moduleB2b.o
    COMMAND ${CMAKE_C_COMPILER} -c moduleB2b.c -o moduleB2b.o -I${CMAKE_SOURCE_DIR}/src -I../../../../ -I../moduleB2a -I../../../moduleC/moduleC1/moduleC1b
    DEPENDS moduleB2b.c
    COMMENT "Compiling moduleB2b.c to moduleB2b.o"
)
add_custom_target(moduleB2b_obj ALL DEPENDS moduleB2b.o) 