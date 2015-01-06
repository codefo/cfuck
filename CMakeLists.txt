cmake_minimum_required(VERSION 2.8.4)
project(cfuck C)

add_library(libbrainfuck STATIC src/brainfuck.c)
set_target_properties(libbrainfuck PROPERTIES PREFIX "")
add_executable(cfuck src/main.c)
target_link_libraries(cfuck libbrainfuck)