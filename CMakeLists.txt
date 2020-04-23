cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(FreeFit CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

configure_file(${CMAKE_SOURCE_DIR}/include/youtubedownloader.in.h ${CMAKE_SOURCE_DIR}/include/youtubedownloader.h)

enable_testing()

add_subdirectory(src)
add_subdirectory(test)