cmake_minimum_required(VERSION 3.15)
project(threads_on_the_bridge C)

set(CMAKE_C_STANDARD 11)

add_executable(threads_on_the_bridge main.c)
target_link_libraries(threads_on_the_bridge pthread)