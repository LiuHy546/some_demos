add_custom_command(
    OUTPUT moduleC1a.o
    COMMAND ${CMAKE_C_COMPILER} -c moduleC1a.c -o moduleC1a.o -I${CMAKE_SOURCE_DIR}/src -I../../../../ -I../moduleC1b
    DEPENDS moduleC1a.c
    COMMENT "Compiling moduleC1a.c to moduleC1a.o"
)
add_custom_target(moduleC1a_obj ALL DEPENDS moduleC1a.o) 