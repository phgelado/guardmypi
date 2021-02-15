
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
#define min_area 500 //pixels

using namespace std;
using namespace cv;


/**
* @class MotionDetect
* @brief Initialise camera and hold motion detection 
*/

class MotionDetect {
	public:
        int opencam();
};

/**
* @class MotionDetect
* @fn opencam
* @brief Opens Camera and video feed
*/

int MotionDetect::opencam()  {

			 /** Create video capturing object
	    	0 opens default camera, otherwise filename as argument 
	    	@param video Video Capturing object
	    	*/
            VideoCapture video(0);

		/// First frame in the video stream
	    	///
	    	/// @param frame is a matrix to save the frame
            Mat frame;
	    Mat grayscale; 
              

            /// Check that video is opened
	        if (!video.isOpened()) return -1;

		/// Loop through available frames
		while (1) {
			
		///Grab the current frame
		video.read(frame);
		
		cvtColor(frame, grayscale, COLOR_RGB2GRAY);
		/// Display the frame
		imshow("Video feed", grayscale);

		/// For breaking the loop
		if (waitKey(25) >= 0) break;

		} /// end while (video.read(frame))
        
	/// Release video capture and write
	video.release(); 

	/// Destroy all windows
	destroyAllWindows();
    return 0;
  }

int main() {
	MotionDetect detector;
	detector.opencam();
	return 0;
}
