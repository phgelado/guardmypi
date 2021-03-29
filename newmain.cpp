/**
* @file         newmain.cpp
* @brief        Testing main with new header file
* @author       Aidan Porteous
*/

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

using namespace std;
using namespace cv;


int main() {
    Mat cameraframe;
    Camera cam;
    sleep(5);
    cam.opencam();
    return 0;
}
