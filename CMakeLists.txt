cmake_minimum_required(VERSION 3.0.0)

project( main )
find_package(nlohmann_json 3.7.0 REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( main main.cpp )
target_link_libraries( main ${OpenCV_LIBS} )

project( rewrite )
find_package(nlohmann_json 3.7.0 REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( rewrite rewrite.cpp )
target_link_libraries( rewrite ${OpenCV_LIBS} )

