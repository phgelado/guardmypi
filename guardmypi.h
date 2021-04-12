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
	Mat ProcessContours(Mat camerafeed); // Motion Detector method
	int flag = 0; //!<Flag variable to invoke ObjectDetector thread t2
};

/**
* @class ObjectDetector
* @brief Used to analyse incoming feed from the camera for different objects such as Pets etc...
*/
class ObjectDetector{
    protected:
		CascadeClassifier cascade; //!< cascade classifier object
        Mat GrayFrame; 	        //!< grayscale frame for processing
		Point pt1;	//!< Start point/coordinate for the object rectangle
	    Point pt2;	//!< End point/coordinate for the object rectangle

	public:
		int flag = -1;
        int loadcascade(String cascadename); // Method for loading particular Haar cascade file
        int detect(Mat ReferenceFrame, double scale_factor, int neighbours,clock_t startTime); // Method for detecting the object in frame

};

/**
* @class Unlock
* @brief Contains various methods to unlock the system. Facial recognition utiises a LBPH algorithm to preict a detected face. 
* QR detection is also implemented using the QRCodeDetector object. 
*/
class Unlock {
    protected: 
        Mat gray,thresh, new_avg, diff,GrayFrame;	//!< Various members to contain a grayscale, threshold, running average and difference frame 
        CascadeClassifier hand_cascade; //!< Instance of the HAAR cascade object
        Scalar color = Scalar(255, 0, 0);
        CascadeClassifier face_cascade;  //!< Instance of the HAAR classifier for facial detection
        Ptr<LBPHFaceRecognizer> recogniser = LBPHFaceRecognizer::create(1,8,8,8,123); //!< Instance of LBPH algorithm
        double confidence = 0.0; //!< Confidence level is altered by the LBPH algorithm of the chances it is the correct face
        string name;			//!< Name of the recognised resident		
		QRCodeDetector qrDecoder; //!< Instance of the QR Code detection object
		Mat bbox, rectifiedImage;	//!< Holds the rectified QR Code and position of the QR Code
		
 
    public:
		//Flags used to trigger new thread creation
        int faceflag = 0;
		int handflag = 0;
		int intruderflag = 0;
		int lockflag = 0;
		int handdetected = 0;
		int QRunlockflag = 0;
		int QRlockflag = 0;
		int resetflag = 0;
		Mat avg;

		int ID = -1;					//Initial LBPH ID
		double secondsPassed = 0.0;		//Number of seconds passed since the current timestamp

        int loadcascade();			//Method used to load all necessary cascade files (xml and yml)
		int face(Mat ReferenceFrame, clock_t startTime);	//Method used to for facial recognition
		int QRUnlock(Mat frame, clock_t startTime);		//Method used to unlock system with QR Code
		int QRLock(Mat frame);		//Method used to lock system using QR Code
		int Deactivate(Mat frame);
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
		int emailflag = 1;  //!< Flag used for email sending
		int pet_timerflag;		//!< Timer flag used solely for pet detection
		int recognise_timerflag;	//!< Human unlocking timer flag used in the unlock methods
		clock_t pet_startTime;		//!< Used to capture a time stamp whenever motion is detected originally
		clock_t recognise_startTime; 	//!< With no pet detetected then a time stamp is taken before running the appropriate unlock method
        
        public:
		
		int opencam(int camport);		//Method used to open up the systems camera and runs an infinite loop of frame capturing	
		int gettime();		//Used to collect the time in GMT 
		int lock(int motionflag, int faceflag, int intruderflag,int QRunlockflag, int QRlockflag);	//Method called to reset all flag variables and re-arm the system
};