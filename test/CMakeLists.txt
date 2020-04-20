include(GoogleTest)

add_executable(unitTests testOne.cpp)

target_include_directories(unitTests PUBLIC /usr/local/include)
target_link_directories(unitTests PUBLIC /usr/local/lib)
target_link_libraries(unitTests gtest)

add_test(NAME test1 COMMAND unitTests)