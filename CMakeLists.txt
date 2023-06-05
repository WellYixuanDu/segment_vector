cmake_minimum_required(VERSION 3.10.0)

project(segment_vector)

set(CMAKE_CXX_STANDARD 11)

set(ACC_TEST_SOURCES 
        acc_test.cc
)

set(SPEED_TEST_SOURCES
        speed_test.cc
)

set(HEADERS
        segment_vector.h
)

add_executable(acc_test ${ACC_TEST_SOURCES})
add_executable(speed_test ${SPEED_TEST_SOURCES})

add_custom_target(format
    COMMAND clang-format -i ${HEADERS}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Formatting code"
)

add_custom_target(check-lint
    COMMAND cppcheck  ${HEADERS}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Running lint checks"
)

add_dependencies(acc_test format check-lint)
add_dependencies(speed_test format check-lint)