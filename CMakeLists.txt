cmake_minimum_required(VERSION 2.8)
project(HumanMotion )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
add_executable( HumanMotion src/Human_motion.cpp )
target_link_libraries( HumanMotion ${OpenCV_LIBS} )


