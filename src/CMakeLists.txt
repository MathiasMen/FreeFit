set(project_sources 
	${CMAKE_SOURCE_DIR}/src/mainwindow.cpp
	${CMAKE_SOURCE_DIR}/src/main.cpp)

set(project_headers
	${CMAKE_SOURCE_DIR}/include/mainwindow.h)

qt5_wrap_cpp(moc_sources ${project_headers})
    
add_executable(FreeFit ${project_sources} ${project_headers} ${moc_sources})
target_link_libraries(FreeFit Qt5::Core Qt5::Gui Qt5::Widgets) 
target_include_directories(FreeFit PUBLIC ${CMAKE_SOURCE_DIR})
