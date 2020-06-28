include(GoogleTest)

set(test_include_dirs /usr/local/include ${CMAKE_SOURCE_DIR})

configure_file(youtubeDownloadTest.in.cpp ${CMAKE_CURRENT_SOURCE_DIR}/youtubeDownloadTest.cpp)
add_executable( YoutubeDownloadTest
                youtubeDownloadTest.cpp
                ${CMAKE_SOURCE_DIR}/src/youtubeoptions.cpp
                ${CMAKE_SOURCE_DIR}/src/youtubedownloader.cpp
                ${CMAKE_SOURCE_DIR}/src/videocutter.cpp)
target_include_directories(YoutubeDownloadTest PUBLIC ${test_include_dirs})
target_link_directories(YoutubeDownloadTest PUBLIC /usr/local/lib)
target_link_libraries(YoutubeDownloadTest gtest)

add_executable( DataTest
                dataTest.cpp
                ${CMAKE_SOURCE_DIR}/src/workout.cpp)
target_include_directories(DataTest PUBLIC ${test_include_dirs})
target_link_directories(DataTest PUBLIC /usr/local/lib)
target_link_libraries(DataTest gtest)

add_executable( DataIOTest
                dataIOTest.cpp
                ${CMAKE_SOURCE_DIR}/src/xmlnode.cpp
                ${CMAKE_SOURCE_DIR}/src/xmlwriter.cpp
                ${CMAKE_SOURCE_DIR}/src/xmlreader.cpp
                ${CMAKE_SOURCE_DIR}/src/parser.cpp)
target_include_directories(DataIOTest PUBLIC ${test_include_dirs})
target_link_directories(DataIOTest PUBLIC /usr/local/lib)
target_link_libraries(DataIOTest gtest)

set(qt_sources 
    ${CMAKE_SOURCE_DIR}/src/mainwindow.cpp
    ${CMAKE_SOURCE_DIR}/src/exerciseeditor.cpp)

set(qt_headers
    ${CMAKE_SOURCE_DIR}/include/mainwindow.h
    ${CMAKE_SOURCE_DIR}/include/controls.h
    ${CMAKE_SOURCE_DIR}/include/exerciseeditor.h
    ${CMAKE_SOURCE_DIR}/include/profileeditor.h
    ${CMAKE_SOURCE_DIR}/include/exerciselistwidget.h
    ${CMAKE_SOURCE_DIR}/include/workoutwidget.h
    ${CMAKE_SOURCE_DIR}/include/workoutgenerationwidget.h
    ${CMAKE_SOURCE_DIR}/include/exerciseviewer.h
    )

qt5_wrap_cpp(moc_sources ${qt_headers})
    
add_executable( ExerciseGUITest
                exerciseGUITest.cpp
                ${CMAKE_SOURCE_DIR}/src/xmlreader.cpp
                ${CMAKE_SOURCE_DIR}/src/xmlwriter.cpp
                ${CMAKE_SOURCE_DIR}/src/xmlnode.cpp
                ${CMAKE_SOURCE_DIR}/src/youtubedownloader.cpp
                ${CMAKE_SOURCE_DIR}/src/youtubeoptions.cpp
                ${CMAKE_SOURCE_DIR}/src/videocutter.cpp
                ${CMAKE_SOURCE_DIR}/src/parser.cpp
                ${CMAKE_SOURCE_DIR}/src/exerciseeditor.cpp
                ${CMAKE_SOURCE_DIR}/src/downloadexercisedemandhandler.cpp
                ${CMAKE_SOURCE_DIR}/src/exerciseviewer.cpp
                ${CMAKE_SOURCE_DIR}/src/exerciselistwidget.cpp
                ${CMAKE_SOURCE_DIR}/src/workout.cpp
                ${CMAKE_SOURCE_DIR}/src/workoutwidget.cpp
                ${CMAKE_SOURCE_DIR}/src/workoutgenerationwidget.cpp
                ${qt_sources} ${qt_headers} ${moc_sources})
target_include_directories(ExerciseGUITest PUBLIC ${test_include_dirs})
target_link_directories(ExerciseGUITest PUBLIC /usr/local/lib)
target_link_libraries(ExerciseGUITest gtest Qt5::Core Qt5::Gui Qt5::Widgets Qt5::WebEngineWidgets Qt5::MultimediaWidgets) 

qt5_wrap_cpp(   profileEditorMocSources
                ${CMAKE_SOURCE_DIR}/include/profileeditor.h
                ${CMAKE_SOURCE_DIR}/include/controls.h)

add_executable( ProfileEditorTest
                profileEditorTest.cpp
                ${profileEditorMocSources}
                ${CMAKE_SOURCE_DIR}/src/xmlnode.cpp
                ${CMAKE_SOURCE_DIR}/src/parser.cpp
                ${CMAKE_SOURCE_DIR}/src/xmlreader.cpp
                ${CMAKE_SOURCE_DIR}/src/xmlwriter.cpp
                )
target_include_directories(ProfileEditorTest PUBLIC ${test_include_dirs})
target_link_directories(ProfileEditorTest PUBLIC /usr/local/lib)
target_link_libraries(ProfileEditorTest gtest Qt5::Core Qt5::Gui Qt5::Widgets) 

qt5_wrap_cpp(   workoutWidgetExerciseListMocSources
                ${CMAKE_SOURCE_DIR}/include/exerciselistwidget.h
                )

add_executable( WorkoutWidgetExerciseListTest
                workoutWidgetExerciseListTest.cpp
                ${workoutWidgetExerciseListMocSources}
                ${CMAKE_SOURCE_DIR}/src/exerciselistwidget.cpp
                ${CMAKE_SOURCE_DIR}/src/workout.cpp
                )
target_include_directories(WorkoutWidgetExerciseListTest PUBLIC ${test_include_dirs})
target_link_directories(WorkoutWidgetExerciseListTest PUBLIC /usr/local/lib)
target_link_libraries(WorkoutWidgetExerciseListTest gtest Qt5::Core Qt5::Gui Qt5::Widgets) 

add_executable( WorkoutGenerationTest
                workoutGenerationTest.cpp
                ${CMAKE_SOURCE_DIR}/src/xmlreader.cpp
                ${CMAKE_SOURCE_DIR}/src/workout.cpp
                ${CMAKE_SOURCE_DIR}/src/xmlnode.cpp
                ${CMAKE_SOURCE_DIR}/src/parser.cpp
                )
target_include_directories(WorkoutGenerationTest PUBLIC ${test_include_dirs})
target_link_directories(WorkoutGenerationTest PUBLIC /usr/local/lib)
target_link_libraries(WorkoutGenerationTest gtest) 

qt5_wrap_cpp(   exerciseViewerMocSources
                ${CMAKE_SOURCE_DIR}/include/exerciseviewer.h
                )

add_executable( ExerciseViewerTest
                exerciseViewerTest.cpp
                ${exerciseViewerMocSources}
                ${CMAKE_SOURCE_DIR}/src/exerciseviewer.cpp
                )
target_include_directories(ExerciseViewerTest PUBLIC ${test_include_dirs})
target_link_directories(ExerciseViewerTest PUBLIC /usr/local/lib)
target_link_libraries(ExerciseViewerTest gtest Qt5::Core Qt5::Gui Qt5::Widgets Qt5::MultimediaWidgets) 

qt5_wrap_cpp(   WorkoutWidgetTestMocSources
                ${CMAKE_SOURCE_DIR}/include/workoutwidget.h
                ${CMAKE_SOURCE_DIR}/include/exerciselistwidget.h
                ${CMAKE_SOURCE_DIR}/include/exerciseviewer.h
                )

add_executable( WorkoutWidgetTest
                workoutWidgetTest.cpp
                ${WorkoutWidgetTestMocSources}
                ${CMAKE_SOURCE_DIR}/src/workoutwidget.cpp
                ${CMAKE_SOURCE_DIR}/src/xmlreader.cpp
                ${CMAKE_SOURCE_DIR}/src/parser.cpp
                ${CMAKE_SOURCE_DIR}/src/xmlnode.cpp
                ${CMAKE_SOURCE_DIR}/src/exerciselistwidget.cpp
                ${CMAKE_SOURCE_DIR}/src/exerciseviewer.cpp
                ${CMAKE_SOURCE_DIR}/src/workout.cpp
                )
target_include_directories(WorkoutWidgetTest PUBLIC ${test_include_dirs})
target_link_directories(WorkoutWidgetTest PUBLIC /usr/local/lib)
target_link_libraries(WorkoutWidgetTest gtest Qt5::Core Qt5::Gui Qt5::Widgets Qt5::MultimediaWidgets) 

qt5_wrap_cpp(   WorkoutGenerationWidgetTestMocSources
                ${CMAKE_SOURCE_DIR}/include/workoutgenerationwidget.h
                ${CMAKE_SOURCE_DIR}/include/controls.h
                )

add_executable( WorkoutGenerationWidgetTest
                workoutGenerationWidgetTest.cpp
                ${WorkoutGenerationWidgetTestMocSources}
                ${CMAKE_SOURCE_DIR}/src/workoutgenerationwidget.cpp
                ${CMAKE_SOURCE_DIR}/src/workout.cpp
                )
target_include_directories(WorkoutGenerationWidgetTest PUBLIC ${test_include_dirs})
target_link_directories(WorkoutGenerationWidgetTest PUBLIC /usr/local/lib)
target_link_libraries(WorkoutGenerationWidgetTest gtest Qt5::Core Qt5::Gui Qt5::Widgets) 

qt5_wrap_cpp(   MaterialWidgets
                ${CMAKE_SOURCE_DIR}/include/materialtextfield.h
                )

add_executable( MaterialWidgetTest
                materialWidgetTest.cpp
                ${MaterialWidgets}
                )
target_include_directories(MaterialWidgetTest PUBLIC ${test_include_dirs})
target_link_directories(MaterialWidgetTest PUBLIC /usr/local/lib)
target_link_libraries(MaterialWidgetTest gtest Qt5::Core Qt5::Gui Qt5::Widgets) 

add_test(NAME YoutubeDownload COMMAND YoutubeDownloadTest)
add_test(NAME Datastructures COMMAND DataTest)
add_test(NAME DataIO COMMAND DataIOTest)
add_test(NAME ExerciseGUI COMMAND ExerciseGUITest)
add_test(NAME ProfileEditor COMMAND ProfileEditorTest)
add_test(NAME WorkoutWidgetExerciseList COMMAND WorkoutWidgetExerciseListTest)
add_test(NAME WorkoutGeneration COMMAND WorkoutGenerationTest)
add_test(NAME WorkoutWidget COMMAND WorkoutWidgetTest)
add_test(NAME ExerciseViewer COMMAND ExerciseViewerTest)
add_test(NAME WorkoutGenerationWidget COMMAND WorkoutGenerationWidgetTest)