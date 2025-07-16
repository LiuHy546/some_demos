add_custom_command(
    OUTPUT moduleB1a.o
    COMMAND ${CMAKE_C_COMPILER} -c moduleB1a.c -o moduleB1a.o -I${CMAKE_SOURCE_DIR}/src -I../../../../ -I../moduleB1b -I../../../moduleC/moduleC2/moduleC2a
    DEPENDS moduleB1a.c
    COMMENT "Compiling moduleB1a.c to moduleB1a.o"
)
add_custom_target(moduleB1a_obj ALL DEPENDS moduleB1a.o) 