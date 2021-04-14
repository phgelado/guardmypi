#define BOOST_TEST_MODULE SqrTests
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <opencv2/core/utility.hpp>
#include "opencv2/objdetect/objdetect.hpp"
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <thread>
#include <ctime>
#include <cstdlib>
#include "guardmypi.h"

Camera Cam;

BOOST_AUTO_TEST_CASE(FailTest)
{
    BOOST_CHECK_EQUAL(-1, Cam.opencam(1));
    BOOST_CHECK(boost::filesystem::exists("/home/aidan/repos/guardmypi/test.jpg"));
    BOOST_CHECK(boost::filesystem::exists("/home/aidan/repos/guardmypi/MyVideo.avi"));
}

BOOST_AUTO_TEST_CASE(PassTest)
{
    BOOST_CHECK_EQUAL(0,Cam.opencam(0));
    BOOST_CHECK(boost::filesystem::exists("/home/aidan/repos/guardmypi/test.jpg"));
    BOOST_CHECK(boost::filesystem::exists("/home/aidan/repos/guardmypi/guardingthepi.yml"));

}