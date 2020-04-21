include(GoogleTest)

add_executable(YoutubeDownloadTest youtubeDownloadTest.cpp)
target_include_directories(YoutubeDownloadTest PUBLIC /usr/local/include)
target_include_directories(YoutubeDownloadTest PUBLIC ${CMAKE_SOURCE_DIR})
target_link_directories(YoutubeDownloadTest PUBLIC /usr/local/lib)
target_link_libraries(YoutubeDownloadTest gtest)

add_test(NAME YoutubeDownload COMMAND YoutubeDownloadTest)