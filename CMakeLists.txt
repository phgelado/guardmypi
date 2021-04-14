cmake_minimum_required(VERSION 2.8)
project(newmain )
find_package( OpenCV REQUIRED )
add_subdirectory (Development)
include_directories( ${OpenCV_INCLUDE_DIRS})
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
<<<<<<< HEAD
add_executable( HumanMotion src/Human_motion.cpp )
target_link_libraries( HumanMotion ${OpenCV_LIBS} )


=======
enable_testing()
add_library (gmp guardmypi.cpp guardmypi.h)
add_executable( main main.cpp)
add_executable(newmain newmain.cpp)
target_link_libraries( main gmp ${OpenCV_LIBS} )
target_link_libraries( newmain gmp ${OpenCV_LIBS} )
add_test (Name Test COMMAND Test)
>>>>>>> Integration
