add_custom_command(
    OUTPUT moduleC2a.o
    COMMAND ${CMAKE_C_COMPILER} -c moduleC2a.c -o moduleC2a.o -I${CMAKE_SOURCE_DIR}/src -I../../../../
    DEPENDS moduleC2a.c
    COMMENT "Compiling moduleC2a.c to moduleC2a.o"
)
add_custom_target(moduleC2a_obj ALL DEPENDS moduleC2a.o) 