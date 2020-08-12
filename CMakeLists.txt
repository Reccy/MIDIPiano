cmake_minimum_required(VERSION 3.15.0)

set(PROJECT_NAME MidiPiano)
set(PROJECT_NAME_SNAKE_CASE midi_piano)
project("${PROJECT_NAME}" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_EXTENSIONS OFF)

#
# Google Test Setup
#
include("cmake/SetupGoogleTest.cmake")

#
# Library Setup
#
set(LIBRARY_DIR "core")
set(LIBRARY_NAME "${PROJECT_NAME}Core")

set(LIBRARY_HEADERS
	"${LIBRARY_DIR}/${PROJECT_NAME_SNAKE_CASE}.h"
)

set(LIBRARY_SOURCES
	"${LIBRARY_DIR}/${PROJECT_NAME_SNAKE_CASE}.cpp"
)

add_library("${LIBRARY_NAME}" STATIC "${LIBRARY_SOURCES}" "${LIBRARY_HEADERS}")

#
# Test Suite Setup
#
set(TEST_SUITE_DIR "test")
set(TEST_RUNNER_EXECUTABLE_NAME "${PROJECT_NAME}TestRunner")
set(TEST_RUNNER_EXECUTABLE_SOURCES
	"${TEST_SUITE_DIR}/test_runner.cpp"
 "test/core/${PROJECT_NAME_SNAKE_CASE}_test.cpp")
add_executable("${TEST_RUNNER_EXECUTABLE_NAME}" "${TEST_RUNNER_EXECUTABLE_SOURCES}")
target_include_directories("${TEST_RUNNER_EXECUTABLE_NAME}" PRIVATE "${LIBRARY_DIR}")
target_link_libraries("${TEST_RUNNER_EXECUTABLE_NAME}" gtest_main "${LIBRARY_NAME}")

#
# Executable Setup
#
set(EXECUTABLE_DIR "main")
set(EXECUTABLE_NAME "${PROJECT_NAME}")

set(EXECUTABLE_SOURCES
	"${EXECUTABLE_DIR}/main.cpp"
)

add_executable("${EXECUTABLE_NAME}" "${EXECUTABLE_SOURCES}")
target_include_directories("${EXECUTABLE_NAME}" PRIVATE "${LIBRARY_DIR}")
target_link_libraries("${EXECUTABLE_NAME}" "${LIBRARY_NAME}")

#
# Compile Flags Setup
#
set(CUSTOM_COMPILE_FLAGS "")
list(APPEND CUSTOM_COMPILE_FLAGS "/W4")		# Highest warning level in MSVC
list(APPEND CUSTOM_COMPILE_FLAGS "/WX")		# Treat warnings as errors
target_compile_options("${LIBRARY_NAME}" PRIVATE "${CUSTOM_COMPILE_FLAGS}")
target_compile_options("${EXECUTABLE_NAME}" PRIVATE "${CUSTOM_COMPILE_FLAGS}")
target_compile_options("${TEST_RUNNER_EXECUTABLE_NAME}" PRIVATE "${CUSTOM_COMPILE_FLAGS}")
