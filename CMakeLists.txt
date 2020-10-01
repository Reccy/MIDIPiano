cmake_minimum_required(VERSION 3.15.0)

set(PROJECT_NAME MidiPiano)
project("${PROJECT_NAME}" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_EXTENSIONS OFF)

#
# Compile Flags Setup
#
set(CUSTOM_COMPILE_FLAGS "")
list(APPEND CUSTOM_COMPILE_FLAGS "/W4")		# Highest warning level in MSVC
list(APPEND CUSTOM_COMPILE_FLAGS "/WX")		# Treat warnings as errors

#
# Core Library Setup
#
set(CORE_LIBRARY_DIR "src/core")
set(CORE_LIBRARY_NAME "${PROJECT_NAME}Core")

set(LIBRARY_HEADERS
	"${CORE_LIBRARY_DIR}/midi_piano.h"
)

set(LIBRARY_SOURCES
	"${CORE_LIBRARY_DIR}/midi_piano.cpp"
)

add_library("${CORE_LIBRARY_NAME}" STATIC "${LIBRARY_SOURCES}" "${LIBRARY_HEADERS}")
target_compile_options("${CORE_LIBRARY_NAME}" PRIVATE "${CUSTOM_COMPILE_FLAGS}")

#
# Third Party Library Setup
#
set(COMMON_THIRD_PARTY_LIBS_DIR "src/lib/commonlibs")

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})
list(APPEND COMMON_THIRD_PARTY_LIBS ${OPENGL_LIBRARIES})

add_subdirectory("${COMMON_THIRD_PARTY_LIBS_DIR}/glad")
list(APPEND COMMON_THIRD_PARTY_LIBS "glad")

#
# Win32 Third Party Library Setup
#
set(WIN32_THIRD_PARTY_LIBS_DIR "src/lib/win32libs")

list(APPEND WIN32_THIRD_PARTY_LIBS "winmm.lib")

add_subdirectory("${WIN32_THIRD_PARTY_LIBS_DIR}/win32_driver_info")
list(APPEND WIN32_THIRD_PARTY_LIBS "win32_driver_info")

add_subdirectory("${WIN32_THIRD_PARTY_LIBS_DIR}/win32_window")
list(APPEND WIN32_THIRD_PARTY_LIBS "win32_window")

add_subdirectory("${WIN32_THIRD_PARTY_LIBS_DIR}/win32_midi")
list(APPEND WIN32_THIRD_PARTY_LIBS "win32_midi")

#
# Test Suite Setup
#
include("cmake/SetupGoogleTest.cmake")
set(TEST_SUITE_DIR "src/test")
set(TEST_RUNNER_EXECUTABLE_NAME "${PROJECT_NAME}TestRunner")
set(TEST_RUNNER_EXECUTABLE_SOURCES
	"${TEST_SUITE_DIR}/test_runner.cpp"
	"${TEST_SUITE_DIR}/core/midi_piano_test.cpp")
add_executable("${TEST_RUNNER_EXECUTABLE_NAME}" "${TEST_RUNNER_EXECUTABLE_SOURCES}")
target_include_directories("${TEST_RUNNER_EXECUTABLE_NAME}" PRIVATE "${CORE_LIBRARY_DIR}")
target_link_libraries("${TEST_RUNNER_EXECUTABLE_NAME}" gtest_main "${CORE_LIBRARY_NAME}")
target_compile_options("${TEST_RUNNER_EXECUTABLE_NAME}" PRIVATE "${CUSTOM_COMPILE_FLAGS}")

enable_testing()
add_test(NAME "${TEST_RUNNER_EXECUTABLE_NAME}" COMMAND "${TEST_RUNNER_EXECUTABLE_NAME}")

#
# Executable Setup
#
set(EXECUTABLE_DIR "src/main")
set(EXECUTABLE_NAME "${PROJECT_NAME}")

list(APPEND INCLUDED_LIBS "${CORE_LIBRARY_NAME}")
list(APPEND INCLUDED_LIBS "${COMMON_THIRD_PARTY_LIBS}")

if (WIN32)
	add_executable("${EXECUTABLE_NAME}" WIN32 "${EXECUTABLE_DIR}/win32_main.cpp")

	list(APPEND INCLUDED_LIBS "${WIN32_THIRD_PARTY_LIBS}")
endif(WIN32)

target_include_directories("${EXECUTABLE_NAME}" PRIVATE "${CORE_LIBRARY_DIR}")
target_link_libraries("${EXECUTABLE_NAME}" "${INCLUDED_LIBS}")
target_compile_options("${EXECUTABLE_NAME}" PRIVATE "${CUSTOM_COMPILE_FLAGS}")
