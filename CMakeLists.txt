cmake_minimum_required(VERSION 2.8.4)
SET(CMAKE_C_FLAGS "-std=c99")
project(cfuck C)

add_library(libbrainfuck STATIC src/brainfuck.c)
set_target_properties(libbrainfuck PROPERTIES PREFIX "")

add_executable(cfuck src/main.c)
target_link_libraries(cfuck libbrainfuck)

enable_testing()

set(EXAMPLES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/examples)
set(TESTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)

add_test(NAME helloworld_test COMMAND cfuck ${EXAMPLES_DIR}/helloworld.b)
set_tests_properties(helloworld_test PROPERTIES PASS_REGULAR_EXPRESSION "Hello, world!")

add_test(NAME rot13_test COMMAND bash -c "./cfuck ${EXAMPLES_DIR}/rot13.b < ${TESTS_DIR}/rot13.txt")
set_tests_properties(rot13_test PROPERTIES PASS_REGULAR_EXPRESSION "Ubj pna lbh gryy na rkgebireg sebz na vagebireg ng AFN\\? In the elevators, the extrovert looks at the OTHER guy's shoes\\.")

add_test(NAME collatz_test COMMAND bash -c "./cfuck ${EXAMPLES_DIR}/collatz.b < ${TESTS_DIR}/collatz.txt")
set_tests_properties(collatz_test PROPERTIES PASS_REGULAR_EXPRESSION "2\n16\n9\n8\n10")

add_test(NAME dbfi_test COMMAND bash -c "./cfuck ${EXAMPLES_DIR}/dbfi.b < ${EXAMPLES_DIR}/helloworld.b")
set_tests_properties(dbfi_test PROPERTIES PASS_REGULAR_EXPRESSION "Hello, world!")

add_test(NAME dvorak_test COMMAND bash -c "./cfuck ${EXAMPLES_DIR}/dvorak.b < ${TESTS_DIR}/dvorak.txt")
set_tests_properties(dvorak_test PROPERTIES PASS_REGULAR_EXPRESSION "',.pyf")

add_test(NAME fib_test COMMAND bash -c "timeout 0.01s ./cfuck ${EXAMPLES_DIR}/fib.b")
set_tests_properties(fib_test PROPERTIES PASS_REGULAR_EXPRESSION "1, 1, 2, 3, 5, 8, 13, 21")
