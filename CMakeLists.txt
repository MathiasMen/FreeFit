cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(FreeFit CXX)

set(CMAKE_PREFIX_PATH /Users/mathias/Qt/5.12.6/clang_64)
find_package(Qt5 COMPONENTS REQUIRED Core Gui Widgets WebEngineWidgets MultimediaWidgets)

set(CMAKE_AUTOMOC ON)
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

ADD_CUSTOM_TARGET(CLEAN
    COMMAND rm ${CMAKE_SOURCE_DIR}/include/youtubedownloader.h
    COMMAND rm ${CMAKE_SOURCE_DIR}/test/dataIOTest.cpp
    COMMAND rm ${CMAKE_SOURCE_DIR}/include/workoutwidget.h
    COMMAND rm ${CMAKE_SOURCE_DIR}/src/downloadexercisedemandhandler.cpp
    COMMAND rm ${CMAKE_SOURCE_DIR}/src/exerciseeditor.cpp
    COMMAND rm ${CMAKE_SOURCE_DIR}/src/main.cpp
    COMMAND rm ${CMAKE_SOURCE_DIR}/src/mainwindow.cpp
    COMMAND rm ${CMAKE_SOURCE_DIR}/test/exerciseGUITest.cpp
    COMMAND rm ${CMAKE_SOURCE_DIR}/test/exerciseViewerTest.cpp
    COMMAND rm ${CMAKE_SOURCE_DIR}/test/profileEditorTest.cpp
    COMMAND rm ${CMAKE_SOURCE_DIR}/test/workoutGenerationTest.cpp
    COMMAND rm ${CMAKE_SOURCE_DIR}/test/workoutGenerationWidgetTest.cpp
    COMMAND rm ${CMAKE_SOURCE_DIR}/test/workoutWidgetExerciseListTest.cpp
    COMMAND rm ${CMAKE_SOURCE_DIR}/test/workoutWidgetTest.cpp
    COMMAND rm ${CMAKE_SOURCE_DIR}/test/input/ExerciseDemand.xml
    COMMAND rm ${CMAKE_SOURCE_DIR}/test/input/LaunchEditor.xml
    COMMAND rm ${CMAKE_SOURCE_DIR}/test/input/ProfileEditorReadInTest.xml
    COMMAND rm ${CMAKE_SOURCE_DIR}/test/input/ReadXMLAndPopulateExerciseList.xml
    COMMAND rm ${CMAKE_SOURCE_DIR}/test/input/WorkoutGenerationTest.xml
    COMMAND rm ${CMAKE_SOURCE_DIR}/test/input/WorkoutWidgetTest.xml
)
    
set(project_sources 
	${CMAKE_SOURCE_DIR}/src/mainwindow.cpp
	${CMAKE_SOURCE_DIR}/src/controls.cpp
	${CMAKE_SOURCE_DIR}/src/downloadexercisedemandhandler.cpp
	${CMAKE_SOURCE_DIR}/src/exerciseeditor.cpp
	${CMAKE_SOURCE_DIR}/src/exerciselistwidget.cpp
	${CMAKE_SOURCE_DIR}/src/exerciseviewer.cpp
	${CMAKE_SOURCE_DIR}/src/materialbutton.cpp
	${CMAKE_SOURCE_DIR}/src/materialclip.cpp
	${CMAKE_SOURCE_DIR}/src/materialslider.cpp
	${CMAKE_SOURCE_DIR}/src/materialtextfield.cpp
	${CMAKE_SOURCE_DIR}/src/parser.cpp
	${CMAKE_SOURCE_DIR}/src/profileeditor.cpp
	${CMAKE_SOURCE_DIR}/src/toggleablelabel.cpp
	${CMAKE_SOURCE_DIR}/src/videocutter.cpp
	${CMAKE_SOURCE_DIR}/src/workout.cpp
	${CMAKE_SOURCE_DIR}/src/workoutwidget.cpp
	${CMAKE_SOURCE_DIR}/src/workoutgenerationwidget.cpp
	${CMAKE_SOURCE_DIR}/src/xmlnode.cpp
	${CMAKE_SOURCE_DIR}/src/xmlreader.cpp
	${CMAKE_SOURCE_DIR}/src/xmlwriter.cpp
	${CMAKE_SOURCE_DIR}/src/youtubedownloader.cpp
	${CMAKE_SOURCE_DIR}/src/youtubeoptions.cpp
	)

set(project_headers
	${CMAKE_SOURCE_DIR}/include/mainwindow.h
	${CMAKE_SOURCE_DIR}/include/controls.h
	${CMAKE_SOURCE_DIR}/include/datadefinitions.h
	${CMAKE_SOURCE_DIR}/include/downloadexercisedemand.h
	${CMAKE_SOURCE_DIR}/include/downloadexercisedemandhandler.h
	${CMAKE_SOURCE_DIR}/include/exercise.h	
	${CMAKE_SOURCE_DIR}/include/exerciseeditor.h
	${CMAKE_SOURCE_DIR}/include/exerciselistwidget.h
	${CMAKE_SOURCE_DIR}/include/exerciseviewer.h
    ${CMAKE_SOURCE_DIR}/include/materialbutton.h
    ${CMAKE_SOURCE_DIR}/include/materialclip.h
    ${CMAKE_SOURCE_DIR}/include/materialdialog.h
    ${CMAKE_SOURCE_DIR}/include/materialslider.h
    ${CMAKE_SOURCE_DIR}/include/materialtextfield.h
    ${CMAKE_SOURCE_DIR}/include/parser.h
	${CMAKE_SOURCE_DIR}/include/profile.h	
	${CMAKE_SOURCE_DIR}/include/profileeditor.h	
	${CMAKE_SOURCE_DIR}/include/toggleablelabel.h
	${CMAKE_SOURCE_DIR}/include/videocutter.h
	${CMAKE_SOURCE_DIR}/include/workout.h
	${CMAKE_SOURCE_DIR}/include/workoutwidget.h
	${CMAKE_SOURCE_DIR}/include/workoutgenerationwidget.h
	${CMAKE_SOURCE_DIR}/include/xmlnode.h
	${CMAKE_SOURCE_DIR}/include/xmlreader.h
	${CMAKE_SOURCE_DIR}/include/xmlwriter.h
	${CMAKE_SOURCE_DIR}/include/youtubedownloader.h
	${CMAKE_SOURCE_DIR}/include/youtubeoptions.h
	)

enable_testing()

add_subdirectory(src)
add_subdirectory(test)
