cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(FreeFit CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

configure_file(${CMAKE_SOURCE_DIR}/include/youtubedownloader.in.h ${CMAKE_SOURCE_DIR}/include/youtubedownloader.h)
configure_file(${CMAKE_SOURCE_DIR}/test/dataIOTest.in.cpp ${CMAKE_SOURCE_DIR}/test/dataIOTest.cpp)

set(cpp_sources ${CMAKE_SOURCE_DIR}/src/videocutter.cpp
    ${CMAKE_SOURCE_DIR}/src/youtubedownloader.cpp
    ${CMAKE_SOURCE_DIR}/src/youtubeoptions.cpp
    ${CMAKE_SOURCE_DIR}/src/xmlnode.cpp
    ${CMAKE_SOURCE_DIR}/src/xmlwriter.cpp)

enable_testing()

add_subdirectory(src)
add_subdirectory(test)