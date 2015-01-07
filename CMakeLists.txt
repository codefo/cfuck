cmake_minimum_required(VERSION 2.8.4)
SET(CMAKE_C_FLAGS "-std=c99")
project(cfuck C)

add_library(libbrainfuck STATIC src/brainfuck.c)
set_target_properties(libbrainfuck PROPERTIES PREFIX "")

add_executable(cfuck src/main.c)
target_link_libraries(cfuck libbrainfuck)

enable_testing()

add_test(NAME helloworld_test_01 COMMAND cfuck
    ${CMAKE_CURRENT_SOURCE_DIR}/examples/helloworld.bf)
set_tests_properties(helloworld_test_01
    PROPERTIES PASS_REGULAR_EXPRESSION "Hello, world!")