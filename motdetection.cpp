
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
* @class MotionDetect
* @brief Initialise camera and hold motion detection 
*/

class MotionDetect {
	public:
        Mat first_frame;
 		Mat frame;
	    Mat frame_diff;
	    Mat grayscale;
	    Mat gray_first;
	    Mat frame_thresh;
		Mat avg;
		Mat scaled_avg;
	    vector<vector<Point> > cnts;
	    Rect rect;
	    Point pt1, pt2;
		/** Create video capturing object
	    	0 opens default camera, otherwise filename as argument 
	    	@param video Video Capturing object
	    	*/
        VideoCapture video;
		int codec = VideoWriter::fourcc('M','J','P','G');
        int opencam();
};

/**
* @class MotionDetect
* @fn opencam
* @brief Opens Camera and video feed
*/

int MotionDetect::opencam()  {

		video.open(0);
		video.set(CAP_PROP_FRAME_WIDTH, 320);
  		video.set(CAP_PROP_FRAME_HEIGHT, 240);
  		//video.set(CAP_PROP_FPS, 2);
  		//video.set(CAP_PROP_FOURCC, codec);

		/// First frame in the video stream
	    	///
	    	/// @param frame is a matrix to save the frame
            	/// Check that video is opened
	        if (!video.isOpened()) return -1;


		///Read the initial frame (background frame)
		
		video.read(first_frame);
	
	/*
		///Convert first frame to grey scale and blur it 
		resize(first_frame, first_frame, Size(21,21));
		cvtColor(first_frame, first_frame, COLOR_RGB2GRAY);

		///Add gaussian smoother to frame
		GaussianBlur(first_frame, first_frame, Size(21,21), 0);
		
	*/
		/// Loop through available frames
		while (1) {
			
		///Grab the current frame
		video.read(frame);

		/// Convert frame to grayscale 
		cvtColor(frame, grayscale, COLOR_RGB2GRAY);

		///Add gaussian smoother to frame
		GaussianBlur(grayscale, grayscale, Size(21,21), 0);


		///Calculate the absolute difference between the first image (background) and current image

		if(avg.empty()==1) {
				grayscale.convertTo(avg, CV_32FC(grayscale.channels()));
		}

		accumulateWeighted(grayscale, avg, 0.5);
		convertScaleAbs(avg, scaled_avg);
		absdiff(grayscale, scaled_avg, frame_diff);
		threshold(frame_diff, frame_thresh, 25, 255, THRESH_BINARY);
				
		dilate(frame_thresh, frame_thresh, Mat(), Point(-1,-1), 2);
        findContours(frame_thresh, cnts, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

		for(int i = 0; i< cnts.size(); i++) {
           		 if(contourArea(cnts[i]) < cnts[i].size()) {
               			 continue;
            			}
			/*
           	 putText(frame, "Motion Detected", Point(10, 20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,0,255),2);
			 rect = boundingRect(cnts[i]);
			 pt1.x =rect.x;
			 pt1.y = rect.y;
			 pt2.x = rect.x + rect.width;
			 pt2.y = rect.y + rect.height;
			 rectangle(frame, pt1, pt2, CV_RGB(255,0,0), 1);
			}	
			*/
			cout << "Motion Detected";	
       		 imshow("Camera", frame);

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
