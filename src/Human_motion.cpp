/**
* @file         motdetection.cpp
* @brief        Motion Detection
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

#define min_area 500 //pixels
using namespace std;
using namespace cv;

/**
* @class MotionDetector
* @brief Analyses video feed from PiCamera to detect motion changes using background substraction
*
*/
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

        // method for loading particular Haar cascade file
        int loadcascade(){
            human_cascade.load("src/haarcascade_upperbody.xml");
            if(!human_cascade.load("src/haarcascade_upperbody.xml"))
           {
                cerr<<"Error Loading XML file"<<endl;
            return 0;
           }    
        }

        // method for detecting humans in frame
        void detect(Mat ReferenceFrame){
            // store original frame as grayscale in gray frame
            cvtColor(ReferenceFrame, GrayFrame, COLOR_BGR2GRAY);
            std::vector<Rect> humans;
            // detect humans in frame - adjust parameters as desired
            human_cascade.detectMultiScale( GrayFrame, humans, 1.5, 2);     
  
            // Draw rectangles on the detected humans
            for( int i = 0; i < humans.size(); i++ )
            {
             rectangle(ReferenceFrame, humans[i], Scalar(0,255,0));
            } 
        }
};


/**
* @class Camera
* @brief Interface with the webcam or videofeed
*/
class Camera {
	protected:
	 MotionDetector detector; //!< Instance of the Motion Detection
	 HumanDetector Hdetector;
	 Mat humanframe, frame;
	 VideoCapture video;	//!< Video Input
	 //Mat frame;		//!< Incoming camera feed	

	public:

		///@brief Opens the camera or webcam atached to the device to be further processed
		///@param frame Is the initial frame to be passed the "frame" from the camera/webcam
		///@returns Doesn't return anything but instead shows the final frame after processing using other methods
		///@see ProcessContours(Mat CameraFeed)
		int opencam();		
};


 int Camera::opencam()  {

		//Open the video feed for the webcam/camera
		video.open(0);
		sleep(3);
		Hdetector.loadcascade();

        // Check that video is opened
	    if (!video.isOpened()) return -1;

		// Loop through available frames
		while (1) {
			
		//Grab the current frame
		video.read(frame);
		humanframe = frame;

		detector.ProcessContours(frame);

		if(detector.flag == 1) {
			thread t1(&HumanDetector::detect, &Hdetector, humanframe);
			t1.join();
			//detector.flag = 0;
		}

		//Show the Video Feed
		imshow("Camera", humanframe);

    
		// For breaking the loop
		if (waitKey(25) >= 0) break;

		} // end while (video.read(frame))
    
	
	//Release video capture and write
	video.release(); 

	// Destroy all windows
	destroyAllWindows();
    return 0;
  }

Mat MotionDetector::ProcessContours(Mat camerafeed) {
	
	//Convert the input image to grayscale
	cvtColor(camerafeed, grayscale, COLOR_RGB2GRAY);

	//Add a smoother to the image  
	GaussianBlur(grayscale, grayscale, Size(21,21), 0);


	/*	Checks if the avg frame is initialised
	*	If not, set the current frame to avg in the same format as the grayscale image
	*	If the background frame is empty convert the current grayscale image to 32 bit float and store in the avg frame
	*/
	if(avg.empty()==1) {
			grayscale.convertTo(avg, CV_32FC(grayscale.channels()));
		}

	//Find weighted average between the current frame and background.
	accumulateWeighted(grayscale, avg, 0.5);

	//Convert from Float to 8-bit absolute value
	//wont allow convertScaleAbs(avg, avg)
	convertScaleAbs(avg, scaled_avg);

	//	Calculate the absolute difference between the current image and the background image
	absdiff(grayscale, scaled_avg, frame_diff);

	//Threshold image for differences between the two frames.
	threshold(frame_diff, frame_thresh, 5, 255, THRESH_BINARY);
				
	//Dilate the threshold image
	dilate(frame_thresh, frame_thresh, Mat(), Point(-1,-1), 2);

	//Contour detection will check the threshold for contours
	//Each contour is stored as a vector of points @param cnts 
    findContours(frame_thresh, cnts, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
 

	for(int i = 0; i< cnts.size(); i++) {
		//Check to see if the contour is too small
        if(contourArea(cnts[i]) < 5000) {
			//cout <<"No Motion\n" << flag;
            continue;
		}
            flag = 1;
			putText(camerafeed, "Motion Detected", Point(10, 20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,0,255),2);
		
			//cout<<"Motion Detected\n" << flag;
					
		}
		
	return camerafeed;
}
 
int main() {
	Mat cameraframe;
	Camera cam;
	MotionDetector detector;
	cam.opencam();
	return 0;
}
