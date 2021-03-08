cmake_minimum_required(VERSION 3.5)

project(1 LANGUAGES C)
set (CMAKE_C_STANDARD 11)

add_executable(1 main.c array.c)
target_link_libraries(1)

add_executable(Test test.c array.c polynomial.c)
target_link_libraries(Test)
