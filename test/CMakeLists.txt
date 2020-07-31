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

add_executable( ExerciseGUITest
                exerciseGUITest.cpp
                ${project_sources}
                ${project_headers}
                ${moc_sources})                
target_include_directories(ExerciseGUITest PUBLIC ${test_include_dirs})
target_link_directories(ExerciseGUITest PUBLIC /usr/local/lib)
target_link_libraries(ExerciseGUITest gtest Qt5::Core Qt5::Gui Qt5::Widgets Qt5::WebEngineWidgets Qt5::MultimediaWidgets) 

add_executable( ProfileEditorTest
                profileEditorTest.cpp
                ${project_sources}
                ${project_headers}
                ${moc_sources})                
target_include_directories(ProfileEditorTest PUBLIC ${test_include_dirs})
target_link_directories(ProfileEditorTest PUBLIC /usr/local/lib)
target_link_libraries(ProfileEditorTest gtest Qt5::Core Qt5::Gui Qt5::Widgets) 

add_executable( WorkoutWidgetExerciseListTest
                workoutWidgetExerciseListTest.cpp
                ${project_sources}
                ${project_headers}
                ${moc_sources})                
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

add_executable( ExerciseViewerTest
                exerciseViewerTest.cpp
                ${exerciseViewerMocSources}
                ${project_sources}
                ${project_headers}
                ${moc_sources})                
target_include_directories(ExerciseViewerTest PUBLIC ${test_include_dirs})
target_link_directories(ExerciseViewerTest PUBLIC /usr/local/lib)
target_link_libraries(ExerciseViewerTest gtest Qt5::Core Qt5::Gui Qt5::Widgets Qt5::MultimediaWidgets) 

add_executable( WorkoutWidgetTest
                workoutWidgetTest.cpp
                ${project_sources}
                ${project_headers}
                ${moc_sources})                
target_include_directories(WorkoutWidgetTest PUBLIC ${test_include_dirs})
target_link_directories(WorkoutWidgetTest PUBLIC /usr/local/lib)
target_link_libraries(WorkoutWidgetTest gtest Qt5::Core Qt5::Gui Qt5::Widgets Qt5::MultimediaWidgets) 

add_executable( WorkoutGenerationWidgetTest
                workoutGenerationWidgetTest.cpp
                ${project_sources}
                ${project_headers}
                ${moc_sources})                
target_include_directories(WorkoutGenerationWidgetTest PUBLIC ${test_include_dirs})
target_link_directories(WorkoutGenerationWidgetTest PUBLIC /usr/local/lib)
target_link_libraries(WorkoutGenerationWidgetTest gtest Qt5::Core Qt5::Gui Qt5::Widgets) 

add_executable( MaterialWidgetTest
                materialWidgetTest.cpp
                ${project_sources}
                ${project_headers}
                ${moc_sources})                
target_include_directories(MaterialWidgetTest PUBLIC ${test_include_dirs})
target_link_directories(MaterialWidgetTest PUBLIC /usr/local/lib)
target_link_libraries(MaterialWidgetTest gtest Qt5::Core Qt5::Gui Qt5::Widgets) 

add_executable( MaterialButtonTest
                materialButtonTest.cpp
                ${project_sources}
                ${project_headers}
                ${moc_sources})                
target_include_directories(MaterialButtonTest PUBLIC ${test_include_dirs})
target_link_directories(MaterialButtonTest PUBLIC /usr/local/lib)
target_link_libraries(MaterialButtonTest gtest Qt5::Core Qt5::Gui Qt5::Widgets) 

add_executable( MaterialSliderTest
                materialSliderTest.cpp
                ${project_sources}
                ${project_headers}
                ${moc_sources})                
target_include_directories(MaterialSliderTest PUBLIC ${test_include_dirs})
target_link_directories(MaterialSliderTest PUBLIC /usr/local/lib)
target_link_libraries(MaterialSliderTest gtest Qt5::Core Qt5::Gui Qt5::Widgets) 

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