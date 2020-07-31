add_executable(app ${project_sources} ${project_headers} ${moc_sources})
target_link_libraries(app Qt5::Core Qt5::Gui Qt5::Widgets Qt5::WebEngineWidgets Qt5::MultimediaWidgets)
target_include_directories(app PUBLIC ${CMAKE_SOURCE_DIR})
