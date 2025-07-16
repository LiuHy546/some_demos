add_custom_command(
    OUTPUT arith.o
    COMMAND ${CMAKE_C_COMPILER} -c arith.c -o arith.o -I${CMAKE_SOURCE_DIR}/src
    DEPENDS arith.c
    COMMENT "Compiling arith.c to arith.o"
)
add_custom_target(arith_obj ALL DEPENDS arith.o) 