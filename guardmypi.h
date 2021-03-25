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

using namespace std;
using namespace cv;

class MotionDetector {
	protected:
	 Mat frame_diff;	//!< Difference output between frames
	 Mat grayscale;	//!< Grayscaled version of the camera frame
	 Mat frame_thresh;	//!< Threshold frame
	 Mat avg;		//!< Background frame
	 Mat scaled_avg;	//!< 8-bit Absolute value frame
	 vector<vector<Point> > cnts;	//!< Vector of points of detected contours
	 Rect rect;	//!<Up-right rectangle to highlight detected contours
	 Point pt1;	//!<Start point/coordinate for the contour rectangle
	 Point pt2;	//!<End point/coordinate for the contour rectangle

	
	public:

	///@brief Takes the incoming frame and performs background subtraction using previous frames.
	///Performs background subtraction to detect changes in the frame i.e. motion. 
	///Subsequently, alters the flag variable equal to 1 if motion is detected to invoke the object detector in a new thread
	///@see HumanDetector::detect Camera:opencam
	///@param camerafeed frame captured from PiCamera or Webcam
	///@returns camerafeed with or without "Motion Detected" text to signify code functioning
	 Mat ProcessContours(Mat camerafeed);

	 //int i = 0;

	 int flag = 0; //!<Flag variable to invoke HumanDetector thread t2
};

class HumanDetector{
    public:

        // cascade classifier object
        CascadeClassifier human_cascade;
        // grayscale frame for processing
        Mat GrayFrame;
		int flag = 0;

        // method for loading particular Haar cascade file
        int loadcascade();
        Mat detect(Mat ReferenceFrame);

};

class Unlock {
    protected: 
        Mat avg,gray,thresh, new_avg, diff;
        CascadeClassifier hand_cascade;
        std::vector<Rect> handvec;
        Scalar color = Scalar(255, 0, 0);
        Point pt1;	//!<Start point/coordinate for the contour rectangle
	    Point pt2;	//!<End point/coordinate for the contour rectangle
		int count = 0;
		clock_t startTime;
		double secondsPassed;

    
    public:
        int flag = 0;
        int loadcascade();
        Mat hand(Mat ReferenceFrame, Mat background);
};

class Camera{
    protected:
        MotionDetector detector; //!< Instance of the Motion Detection
        Unlock recognise;
	    HumanDetector Hdetector;
	    Mat humanframe, frame, testframe, background, handframe;
	    VideoCapture video;	//!< Video Input
	    //Mat frame;		//!< Incoming camera feed	
        
        public:

		///@brief Opens the camera or webcam atached to the device to be further processed
		///@param frame Is the initial frame to be passed the "frame" from the camera/webcam
		///@returns Doesn't return anything but instead shows the final frame after processing using other methods
		///@see ProcessContours(Mat CameraFeed)
		int opencam();		

};