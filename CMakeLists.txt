cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(FreeFit CXX)

set(CMAKE_PREFIX_PATH /Users/mathias/Qt/5.12.6/clang_64)
find_package(Qt5 COMPONENTS REQUIRED Core Gui Widgets WebEngineWidgets MultimediaWidgets)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

configure_file(${CMAKE_SOURCE_DIR}/include/youtubedownloader.in.h ${CMAKE_SOURCE_DIR}/include/youtubedownloader.h)
configure_file(${CMAKE_SOURCE_DIR}/test/dataIOTest.in.cpp ${CMAKE_SOURCE_DIR}/test/dataIOTest.cpp)
configure_file(${CMAKE_SOURCE_DIR}/include/workoutwidget.in.h ${CMAKE_SOURCE_DIR}/include/workoutwidget.h)
configure_file(${CMAKE_SOURCE_DIR}/src/downloadexercisedemandhandler.in.cpp ${CMAKE_SOURCE_DIR}/src/downloadexercisedemandhandler.cpp)
configure_file(${CMAKE_SOURCE_DIR}/src/exerciseeditor.in.cpp ${CMAKE_SOURCE_DIR}/src/exerciseeditor.cpp)
configure_file(${CMAKE_SOURCE_DIR}/src/main.in.cpp ${CMAKE_SOURCE_DIR}/src/main.cpp)
configure_file(${CMAKE_SOURCE_DIR}/src/mainwindow.in.cpp ${CMAKE_SOURCE_DIR}/src/mainwindow.cpp)

set(cpp_sources ${CMAKE_SOURCE_DIR}/src/videocutter.cpp
    ${CMAKE_SOURCE_DIR}/src/youtubedownloader.cpp
    ${CMAKE_SOURCE_DIR}/src/youtubeoptions.cpp
    ${CMAKE_SOURCE_DIR}/src/xmlnode.cpp
    ${CMAKE_SOURCE_DIR}/src/xmlwriter.cpp)

enable_testing()

add_subdirectory(src)
add_subdirectory(test)
