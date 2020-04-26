include(GoogleTest)

set(test_include_dirs /usr/local/include ${CMAKE_SOURCE_DIR})

configure_file(youtubeDownloadTest.in.cpp ${CMAKE_CURRENT_SOURCE_DIR}/youtubeDownloadTest.cpp)
add_executable(YoutubeDownloadTest youtubeDownloadTest.cpp ${cpp_sources})
target_include_directories(YoutubeDownloadTest PUBLIC ${test_include_dirs})
target_link_directories(YoutubeDownloadTest PUBLIC /usr/local/lib)
target_link_libraries(YoutubeDownloadTest gtest)

add_executable(DataTest dataTest.cpp ${cpp_sources})
target_include_directories(DataTest PUBLIC ${test_include_dirs})
target_link_directories(DataTest PUBLIC /usr/local/lib)
target_link_libraries(DataTest gtest)

add_executable(DataIOTest dataIOTest.cpp ${cpp_sources})
target_include_directories(DataIOTest PUBLIC ${test_include_dirs})
target_link_directories(DataIOTest PUBLIC /usr/local/lib)
target_link_libraries(DataIOTest gtest)

add_test(NAME YoutubeDownload COMMAND YoutubeDownloadTest)
add_test(NAME Datastructures COMMAND DataTest)
add_test(NAME DataIO COMMAND DataIOTest)
