cmake_minimum_required(VERSION 3.1.0)

project(MidiPiano LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

#
# Google Test Setup
#
include("cmake/SetupGoogleTest.cmake")

#
# Library Setup
#
set(LIBRARY_DIR "core")
set(LIBRARY_NAME "MidiPianoCore")

set(LIBRARY_HEADERS
	"${LIBRARY_DIR}/midi_piano.h"
)

set(LIBRARY_SOURCES
	"${LIBRARY_DIR}/midi_piano.cpp"
)

add_library("${LIBRARY_NAME}" STATIC "${LIBRARY_SOURCES}" "${LIBRARY_HEADERS}")


#
# Test Suite Setup
#
set(TEST_SUITE_DIR "test")
set(TEST_RUNNER_EXECUTABLE_NAME "MidiPianoTestRunner")
set(TEST_RUNNER_EXECUTABLE_SOURCES
	"${TEST_SUITE_DIR}/test_runner.cpp"
 "test/core/midi_piano_test.cpp")
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
