cmake_minimum_required(VERSION 2.8)
project(newmain )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS})
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
<<<<<<< HEAD
add_executable( newmain guardmypi.cpp guardmypi.hpp newmain.cpp )
target_link_libraries( newmain ${OpenCV_LIBS} )
=======
add_executable( HumanMotion src/Human_motion.cpp )
target_link_libraries( HumanMotion ${OpenCV_LIBS} )
>>>>>>> 64f2ea76601cda35cd7ed747f393457085c8bca6
