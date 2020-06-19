set(project_sources 
	${CMAKE_SOURCE_DIR}/src/main.cpp
	${CMAKE_SOURCE_DIR}/src/mainwindow.cpp
	${CMAKE_SOURCE_DIR}/src/downloadexercisedemandhandler.cpp
	${CMAKE_SOURCE_DIR}/src/exerciseeditor.cpp
	${CMAKE_SOURCE_DIR}/src/exerciselistwidget.cpp
	${CMAKE_SOURCE_DIR}/src/exerciseviewer.cpp
	${CMAKE_SOURCE_DIR}/src/parser.cpp
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
	${CMAKE_SOURCE_DIR}/include/parser.h
	${CMAKE_SOURCE_DIR}/include/profile.h	
	${CMAKE_SOURCE_DIR}/include/profileeditor.h	
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

qt5_wrap_cpp(moc_sources
	${CMAKE_SOURCE_DIR}/include/exerciseeditor.h
	${CMAKE_SOURCE_DIR}/include/exerciseviewer.h
	${CMAKE_SOURCE_DIR}/include/exerciselistwidget.h
	${CMAKE_SOURCE_DIR}/include/mainwindow.h
	${CMAKE_SOURCE_DIR}/include/controls.h
	${CMAKE_SOURCE_DIR}/include/profileeditor.h
	${CMAKE_SOURCE_DIR}/include/workoutwidget.h	
	${CMAKE_SOURCE_DIR}/include/workoutgenerationwidget.h		
	)
    
add_executable(app ${project_sources} ${project_headers} ${moc_sources})
target_link_libraries(app Qt5::Core Qt5::Gui Qt5::Widgets Qt5::WebEngineWidgets Qt5::MultimediaWidgets)
target_include_directories(app PUBLIC ${CMAKE_SOURCE_DIR})
