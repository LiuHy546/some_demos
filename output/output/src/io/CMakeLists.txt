add_custom_command(
    OUTPUT fileio.o
    COMMAND ${CMAKE_C_COMPILER} -c fileio.c -o fileio.o -I${CMAKE_SOURCE_DIR}/src
    DEPENDS fileio.c
    COMMENT "Compiling fileio.c to fileio.o"
)
add_custom_target(fileio_obj ALL DEPENDS fileio.o) 