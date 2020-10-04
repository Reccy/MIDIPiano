cmake_minimum_required(VERSION 3.15.0)
project(win32_midi)

add_library(win32_midi "win32_midi.h" "win32_midi.cpp" "win32_create_midi_exception.h" "win32_note.h" "win32_note.cpp")
target_include_directories(win32_midi PUBLIC ".")

target_link_libraries(win32_midi PRIVATE win32_driver_info)
