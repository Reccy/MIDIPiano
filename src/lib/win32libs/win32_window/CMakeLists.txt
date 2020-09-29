cmake_minimum_required(VERSION 3.15.0)
project(win32_window)

add_library(win32_window STATIC "win32_window.h" "win32_window.cpp" "win32_create_window_exception.h")
target_include_directories(win32_window PUBLIC ".")