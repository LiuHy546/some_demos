add_custom_command(
    OUTPUT log.o
    COMMAND ${CMAKE_C_COMPILER} -c log.c -o log.o -I${CMAKE_SOURCE_DIR}/src
    DEPENDS log.c
    COMMENT "Compiling log.c to log.o"
)
add_custom_target(log_obj ALL DEPENDS log.o) 