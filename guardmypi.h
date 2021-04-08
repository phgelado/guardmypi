#include <opencv2/core/utility.hpp>
#include "opencv2/objdetect/objdetect.hpp"
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
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

/**
* @class MotionDetector
* @brief Analyses video feed from PiCamera to detect motion changes 
*/
class MotionDetector {
	protected:
	 Mat frame_diff;	//!< Difference output between frames
	 Mat grayscale;	//!< Grayscaled version of the camera frame
	 Mat frame_thresh;	//!< Threshold frame
	 Mat scaled_avg;	//!< 8-bit Absolute value frame
	 vector<vector<Point> > cnts;	//!< Vector of points of detected contours
	 Rect rect;	//!<Up-right rectangle to highlight detected contours
	 Point pt1;	//!<Start point/coordinate for the contour rectangle
	 Point pt2;	//!<End point/coordinate for the contour rectangle
	
	public:
	Mat avg; //!< Running average of the camerafeed 
	Mat ProcessContours(Mat camerafeed);
	int flag = 0; //!<Flag variable to invoke ObjectDetector thread t2
};

/**
* @class ObjectDetector
* @brief Used to analyse incoming feed from the camera for different objects such as Pets etc...
*/
class ObjectDetector{
    protected:
        // cascade classifier object
		CascadeClassifier cascade;
        // grayscale frame for processing
        Mat GrayFrame;
		Point pt1;	//!<Start point/coordinate for the contour rectangle
	    Point pt2;	//!<End point/coordinate for the contour rectangle

	public:
		int flag = -1;
        // method for loading particular Haar cascade file
        int loadcascade(String cascadename);
        int detect(Mat ReferenceFrame, double scale_factor, int neighbours,clock_t startTime);

};

/**
* @class Unlock
* @brief Contains various methods to unlock the system 
*/
class Unlock {
    protected: 
        Mat gray,thresh, new_avg, diff;
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
		QRCodeDetector qrDecoder;
		Mat bbox, rectifiedImage;
		
 
    public:
        int faceflag = 0;
		int handflag = 0;
		int intruderflag = 0;
		int lockflag = 0;
		int handdetected = 0;
		int QRunlockflag = 0;
		int QRlockflag = 0;
		int resetflag = 0;
		Mat avg;

		int ID = -1;
		double secondsPassed = 0.0;

        int loadcascade();
		int face(Mat ReferenceFrame, clock_t startTime);
		int QRUnlock(Mat frame, clock_t startTime);
		int QRLock(Mat frame);
};

/**
* @class Camera
* @brief Interfaces in the main program to open/close the camera and create new threads
*/
class Camera{
    protected:
        MotionDetector motiondetector; //!< Instance of the Motion Detection
        Unlock recognise;			//!< Instance of the unlock class with uses of facial recognition, QR Detection
		ObjectDetector petdetector; //!< Instance of the object detector that will be accessing the ped detection method
	    Mat frame, testframe; //!< Various Mat variables assigned to the video feed and empty testframe used to reset frames
	    VideoCapture video;		//!< Video input capture object
		int hour;			//!< Hour time in GMT	
		int pet_timerflag;		//!< Timer flag used solely for pet detection
		int recognise_timerflag;	//!< Human unlocking timer flag used in the unlock methods
		clock_t pet_startTime;		//!< Used to capture a time stamp whenever motion is detected originally
		clock_t recognise_startTime; 	//!< With no pet detetected then a time stamp is taken before running the appropriate unlock method
        
        public:
		
		int opencam();
		int gettime();
		int lock(int motionflag, int faceflag, int intruderflag,int QRunlockflag, int QRlockflag);
};