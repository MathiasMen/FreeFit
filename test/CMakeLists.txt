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
	${CMAKE_SOURCE_DIR}/src/mainwindow.cpp)

set(qt_headers
    ${CMAKE_SOURCE_DIR}/include/mainwindow.h
    ${CMAKE_SOURCE_DIR}/include/exerciseeditor.h)

qt5_wrap_cpp(moc_sources ${qt_headers})
    
add_executable( ExerciseGUITest
                exerciseGUITest.cpp
                ${CMAKE_SOURCE_DIR}/src/xmlreader.cpp
                ${CMAKE_SOURCE_DIR}/src/xmlwriter.cpp
                ${CMAKE_SOURCE_DIR}/src/xmlnode.cpp
                ${CMAKE_SOURCE_DIR}/src/youtubedownloader.cpp
                ${CMAKE_SOURCE_DIR}/src/youtubeoptions.cpp
                ${CMAKE_SOURCE_DIR}/src/videocutter.cpp
                ${qt_sources} ${qt_headers} ${moc_sources})
target_include_directories(ExerciseGUITest PUBLIC ${test_include_dirs})
target_link_directories(ExerciseGUITest PUBLIC /usr/local/lib)
target_link_libraries(ExerciseGUITest gtest Qt5::Core Qt5::Gui Qt5::Widgets) 

add_test(NAME YoutubeDownload COMMAND YoutubeDownloadTest)
add_test(NAME Datastructures COMMAND DataTest)
add_test(NAME DataIO COMMAND DataIOTest)
add_test(NAME ExerciseGUI COMMAND ExerciseGUITest)