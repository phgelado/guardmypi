cmake_minimum_required(VERSION 2.8)
project(newmain )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS})
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
<<<<<<< HEAD
add_executable( newmain newmain.cpp guardmypi.cpp)
=======
add_executable( newmain newmain.cpp guardmypi.cpp )
>>>>>>> f06523b3e8c11d6b8f90e983c850a4541ad2c7c7
target_link_libraries( newmain ${OpenCV_LIBS} )
