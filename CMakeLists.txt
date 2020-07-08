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
configure_file(${CMAKE_SOURCE_DIR}/test/exerciseGUITest.in.cpp ${CMAKE_SOURCE_DIR}/test/exerciseGUITest.cpp)
configure_file(${CMAKE_SOURCE_DIR}/test/exerciseViewerTest.in.cpp ${CMAKE_SOURCE_DIR}/test/exerciseViewerTest.cpp)
configure_file(${CMAKE_SOURCE_DIR}/test/profileEditorTest.in.cpp ${CMAKE_SOURCE_DIR}/test/profileEditorTest.cpp)
configure_file(${CMAKE_SOURCE_DIR}/test/workoutGenerationTest.in.cpp ${CMAKE_SOURCE_DIR}/test/workoutGenerationTest.cpp)
configure_file(${CMAKE_SOURCE_DIR}/test/workoutGenerationWidgetTest.in.cpp ${CMAKE_SOURCE_DIR}/test/workoutGenerationWidgetTest.cpp)
configure_file(${CMAKE_SOURCE_DIR}/test/workoutWidgetExerciseListTest.in.cpp ${CMAKE_SOURCE_DIR}/test/workoutWidgetExerciseListTest.cpp)
configure_file(${CMAKE_SOURCE_DIR}/test/workoutWidgetTest.in.cpp ${CMAKE_SOURCE_DIR}/test/workoutWidgetTest.cpp)
configure_file(${CMAKE_SOURCE_DIR}/test/input/ExerciseDemand.in.xml ${CMAKE_SOURCE_DIR}/test/input/ExerciseDemand.xml)
configure_file(${CMAKE_SOURCE_DIR}/test/input/LaunchEditor.in.xml ${CMAKE_SOURCE_DIR}/test/input/LaunchEditor.xml)
configure_file(${CMAKE_SOURCE_DIR}/test/input/ProfileEditorReadInTest.in.xml ${CMAKE_SOURCE_DIR}/test/input/ProfileEditorReadInTest.xml)
configure_file(${CMAKE_SOURCE_DIR}/test/input/ReadXMLAndPopulateExerciseList.in.xml ${CMAKE_SOURCE_DIR}/test/input/ReadXMLAndPopulateExerciseList.xml)
configure_file(${CMAKE_SOURCE_DIR}/test/input/WorkoutGenerationTest.in.xml ${CMAKE_SOURCE_DIR}/test/input/WorkoutGenerationTest.xml)
configure_file(${CMAKE_SOURCE_DIR}/test/input/WorkoutWidgetTest.in.xml ${CMAKE_SOURCE_DIR}/test/input/WorkoutWidgetTest.xml)

set(cpp_sources ${CMAKE_SOURCE_DIR}/src/videocutter.cpp
    ${CMAKE_SOURCE_DIR}/src/youtubedownloader.cpp
    ${CMAKE_SOURCE_DIR}/src/youtubeoptions.cpp
    ${CMAKE_SOURCE_DIR}/src/xmlnode.cpp
    ${CMAKE_SOURCE_DIR}/src/xmlwriter.cpp)

enable_testing()

add_subdirectory(src)
add_subdirectory(test)
