add_custom_command(
    OUTPUT socket.o
    COMMAND ${CMAKE_C_COMPILER} -c socket.c -o socket.o -I${CMAKE_SOURCE_DIR}/src
    DEPENDS socket.c
    COMMENT "Compiling socket.c to socket.o"
)
add_custom_target(socket_obj ALL DEPENDS socket.o) 