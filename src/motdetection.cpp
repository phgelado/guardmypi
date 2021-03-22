/**
* @file         motdetection.cpp
* @brief        Motion Detection
* @author       Aidan Porteous
*/

#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <cstring>
#include <unistd.h>
#define min_area 500 //pixels

using namespace std;
using namespace cv;

/**
* @class MotionDetector
* @brief Analyses video feed from PiCamera to detect motion changes 
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
		Mat ProcessContours(Mat camerafeed);
};

/**
* @class Camera
* @brief Interface with the webcam or videofeed
*/
class Camera {
	protected:
	MotionDetector detector; //!< Instance of the Motion Detection
	VideoCapture video;	//!< Video Input
	Mat frame;		//!< Incoming camera feed	

	public:
		int opencam();
};

/**
* @class Camera
* @fn opencam
* @brief Opens Camera and transmits video feed
*/

int Camera::opencam()  {

		//Open the video feed for the webcam/camera
		video.open(0);

		//Set width and height of the video feed
		

        // Check that video is opened
	    if (!video.isOpened()) return -1;

		// Loop through available frames
		while (1) {
			
		//Grab the current frame
		video.read(frame);

		detector.ProcessContours(frame);

		//video.set(CAP_PROP_FRAME_WIDTH,480);
  		//video.set(CAP_PROP_FRAME_HEIGHT, 480);
       	
		//Show the Video Feed
		imshow("Camera", frame);

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
	
	cvtColor(camerafeed, grayscale, COLOR_RGB2GRAY);

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
	threshold(frame_diff, frame_thresh, 1, 255, THRESH_BINARY);
				
	//Dilate the threshold image
	dilate(frame_thresh, frame_thresh, Mat(), Point(-1,-1), 2);

	//Contour detection will check the threshold for contours
	//Each contour is stored as a vector of points @param cnts 
    findContours(frame_thresh, cnts, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
 

	for(int i = 0; i< cnts.size(); i++) {
		//Check to see if the contour is too small
        f(contourArea(cnts[i]) < 5000) 
            continue;
            		
			putText(camerafeed, "Motion Detected", Point(10, 20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,0,255),2);
			rect = boundingRect(cnts[i]);	// Contains coordinates of bounding rectangle for detected contours 
			pt1.x = rect.x;					// Origin point of rectangle on the x-axis 
			pt1.y = rect.y;					// Origin point of rectangle on the y-axis 
			pt2.x = rect.x + rect.width;	// Final point along x-axis 
			pt2.y = rect.y + rect.height;	//Final point along y-axis 

			//Draws rectangle using start and stop coorinates
			rectangle(camerafeed, pt1, pt2, CV_RGB(255,0,0), 2);		
		}
		
	return camerafeed;
}
 
int main() {
	Camera feed;
	feed.opencam();
	return 0;
}
