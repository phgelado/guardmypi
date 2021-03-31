#include <opencv2/core/utility.hpp>
#include "opencv2/objdetect/objdetect.hpp"
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/face.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <thread>
#include <ctime>
#include <cstdlib>

using namespace std;
using namespace cv;
using namespace cv::face;

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
	///@see ObjectDetector::detect Camera:opencam
	///@param camerafeed frame captured from PiCamera or Webcam
	///@returns camerafeed with or without "Motion Detected" text to signify code functioning
	 Mat ProcessContours(Mat camerafeed);

	 //int i = 0;

	 int flag = 0; //!<Flag variable to invoke ObjectDetector thread t2
};

class ObjectDetector{
    public:

        // cascade classifier object
		CascadeClassifier cascade;
        // grayscale frame for processing
        Mat GrayFrame;
		int flag = 0;
		Point pt1;	//!<Start point/coordinate for the contour rectangle
	    Point pt2;	//!<End point/coordinate for the contour rectangle

        // method for loading particular Haar cascade file
        int loadcascade(String cascadename);
        Mat detect(Mat ReferenceFrame, double scale_factor, int neighbours);

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
		
		// cascade classifier object
        CascadeClassifier face_cascade;
        Ptr<LBPHFaceRecognizer> recogniser = LBPHFaceRecognizer::create(1,8,8,8,123);
        double confidence = 0.0;
        // grayscale frame for processing
        Mat GrayFrame;
        string name;
        int area;
		
 
    public:
        int faceflag = 0;
		int handflag = 0;
		int intruderflag = 0;
		int lockflag = 0;
		int handdetected = 0;
		int ID = -1;
		double secondsPassed = 0.0;

        int loadcascade();
        int hand(Mat ReferenceFrame, Mat background);
		int face(Mat ReferenceFrame, clock_t startTime);
};


class Lock {
	public:
	int lock(int lockflag, int humanflag, int motionflag);
};

class Camera{
    protected:
        MotionDetector motiondetector; //!< Instance of the Motion Detection
        Unlock recognise;
	    ObjectDetector humandetector;
		ObjectDetector petdetector;
		Lock locksystem;
	    Mat humanframe, frame, testframe, background, handframe;
	    VideoCapture video;	//!< Video Input
		int hour;
	    //Mat frame;		//!< Incoming camera feed	

        
        public:

		///@brief Opens the camera or webcam atached to the device to be further processed
		///@param frame Is the initial frame to be passed the "frame" from the camera/webcam
		///@returns Doesn't return anything but instead shows the final frame after processing using other methods
		///@see ProcessContours(Mat CameraFeed)
		int opencam();
		int gettime();
		int lock(int lockflag, int unlockflag, int motionflag, int intruderflag);
		//int lock(int lockflag, int unlockflag, int motionflag, int intruderflag);		

};

