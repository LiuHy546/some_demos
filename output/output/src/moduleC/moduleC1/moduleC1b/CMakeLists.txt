add_custom_command(
    OUTPUT moduleC1b.o
    COMMAND ${CMAKE_C_COMPILER} -c moduleC1b.c -o moduleC1b.o -I${CMAKE_SOURCE_DIR}/src -I../../../../
    DEPENDS moduleC1b.c
    COMMENT "Compiling moduleC1b.c to moduleC1b.o"
)
add_custom_target(moduleC1b_obj ALL DEPENDS moduleC1b.o) 