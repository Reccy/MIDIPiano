cmake_minimum_required(VERSION 3.15.0)
project(win32_driver_info)

add_library(win32_driver_info "win32_driver_info.h" "win32_driver_info.cpp")
target_include_directories(win32_driver_info PUBLIC ".")