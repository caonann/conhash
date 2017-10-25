cmake_minimum_required (VERSION 2.8)
project (conhash)
add_definitions(-std=c++11)

include_directories ("${PROJECT_SOURCE_DIR}/include")
add_subdirectory(src)
add_subdirectory(example)

#target_link_libraries(conhash conhash.cpp)

