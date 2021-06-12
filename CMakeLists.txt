cmake_minimum_required(VERSION 3.19)
project(progetto_bd C)

set(CMAKE_C_STANDARD 99)

add_executable(progetto_bd main.c defines.h admin.c manager.c)
target_link_libraries(progetto_bd mysqlclient)