cmake_minimum_required(VERSION 4.1)
project(UDPSocket C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBPQ REQUIRED libpq)

add_executable(UDPSocket src/main.c
        src/protocol/protocol.h
        src/net/net.c
        src/net/net.h
        src/game/player.c
        src/game/player.h
        src/game/game.c
        src/game/game.h
        src/util/time.c
        src/util/time.h
        )

target_include_directories(UDPSocket PRIVATE ${LIBPQ_INCLUDE_DIRS})
target_link_libraries(UDPSocket PRIVATE ${LIBPQ_LIBRARIES})