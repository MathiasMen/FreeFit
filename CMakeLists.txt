cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(FreeFit CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

enable_testing()

add_subdirectory(src)