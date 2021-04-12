find_package (Boost COMPONENTS system filesystem unit_test_framework REQUIRED)
include_directories (${Boost_INCLUDE_DIRS}
                    ${DEMO_SOURCE_DIR}
                    )
include_directories( ${OpenCV_INCLUDE_DIRS})
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")


add_definitions (-DBOOST_TEST_DYN_LINK)
add_executable (Test test.cpp)
target_link_libraries (Test
                       gmp
                       ${Boost_FILESYSTEM_LIBRARY}
                       ${Boost_SYSTEM_LIBRARY}
                       ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                       ${OpenCV_LIBS}
                       )
add_test (NAME Test COMMAND Test)
