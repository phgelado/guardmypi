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
#include <ctime>
#include <cstdlib>
#include </home/aidan/repos/guardmypi/guardmypi.h>


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
		int flag = 0;

        // method for loading particular Haar cascade file
        int loadcascade(){
            human_cascade.load("haarcascade_frontalface_default.xml");
            if(!human_cascade.load("haarcascade_frontalface_default.xml"))
           {
                cerr<<"Error Loading XML file"<<endl;
            return 0;
           }    
        }

        // method for detecting humans in frame
        Mat detect(Mat ReferenceFrame){
            // store original frame as grayscale in gray frame
            cvtColor(ReferenceFrame, GrayFrame, COLOR_BGR2GRAY);
            std::vector<Rect> humans;
            // detect humans in frame - adjust parameters as desired
            human_cascade.detectMultiScale( GrayFrame, humans, 1.5, 2);     
  
            // Draw rectangles on the detected humans
            for( int i = 0; i < humans.size(); i++ )
            {
			 flag = 1;
             rectangle(ReferenceFrame, humans[i], Scalar(0,255,0));
			 
            }
			return ReferenceFrame; 
        }
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
        int loadcascade(){
            hand_cascade.load("Hand_haar_cascade.xml");
            
            if(!hand_cascade.load("Hand_haar_cascade.xml"))
           {
                cout<<"Error Loading XML file"<<endl;
            return 0;
           }    
        }

        Mat hand(Mat ReferenceFrame, Mat background) {
            cvtColor(ReferenceFrame, gray, COLOR_RGB2GRAY);
        	GaussianBlur(gray, gray, Size(21,21), 0);
            if(avg.empty()==1) {
			    gray.convertTo(avg, CV_32FC(gray.channels()));
		        } 

	        convertScaleAbs(background, new_avg);

	       //Calculate the absolute difference between the current image and the background image
	        absdiff(gray, new_avg, diff);

	        //Threshold image for differences between the two frames.
	        threshold(diff, thresh, 50, 255, THRESH_BINARY);
				
	        //Dilate the threshold image
	        dilate(thresh, thresh, Mat(), Point(-1,-1), 2); 
        	hand_cascade.detectMultiScale(thresh, handvec, 1.1, 8);  
			
	///////////////////////////////////////////////////////////////////////////////
	//				Execute timer that waits X seconds for a hand detection
	//////////////////////////////////////////////////////////////////////////////
			//Timer implementation NEEDS FIXED only returns 0s 
			if(handvec.size() < 1 && secondsPassed < 2) {
				startTime = clock(); //Start timer
				secondsPassed = secondsPassed = (clock() - startTime) / CLOCKS_PER_SEC;
				}

	/////////////////////////////////////////////////////////////////////////////
	//	Execute this if no hand or QR code is detected aka the intruder push notification stuff
	/////////////////////////////////////////////////////////////////////////////
			else if(secondsPassed >= 2) {
				cout << "INTRUDER" << "\n";
				//Escape the function here and set an intruder flag? break; or return 0 to terminate the function then do t2.join()?
			}

			for( int i = 0; i < handvec.size(); i++)
            	{
                	Rect rect = handvec[i];
                	pt1.x = rect.x;					// Origin point of rectangle on the x-axis 
			    	pt1.y = rect.y;					// Origin point of rectangle on the y-axis 
			    	pt2.x = rect.x + rect.width;	// Final point along x-axis 
			    	pt2.y = rect.y + rect.height;	//Final point along y-axis 
                	rectangle(ReferenceFrame, pt1,pt2, color, 3, 2, 0);
					putText(ReferenceFrame, "Hand Detected", Point(100, 50), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,0,255),2);
                } 
		
        return thresh;
          
        }
};


/**
* @class Camera
* @brief Interface with the webcam or videofeed
*/
class Camera {
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


 int Camera::opencam()  {

		//Open the video feed for the webcam/camera
		video.open(0);
		sleep(3);
		Hdetector.loadcascade();
		recognise.loadcascade();
        video.read(background);
        cvtColor(background, background, COLOR_RGB2GRAY);
        GaussianBlur(background, background, Size(21,21), 0);

        // Check that video is opened
	    if (!video.isOpened()) return -1;

		// Loop through available frames
		while (1) {
			
		//Grab the current frame
		video.read(frame);
		//resize(frame,frame,Size(340,200));
		
		humanframe = frame;
		handframe = frame;

		detector.ProcessContours(frame);

		if(detector.flag == 1) {
			thread t1(&HumanDetector::detect, &Hdetector, frame);
			t1.join();
		}

		if (Hdetector.flag == 1) {
			thread t2(&Unlock::hand, &recognise, frame, background);
			t2.join();
		}
		//Show the Video Feed
		imshow("Camera", frame);


		//Reset function probably gonna go in here. Code only works properly when I put flag resets here!
		detector.flag = 0;
		Hdetector.flag =0;
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
