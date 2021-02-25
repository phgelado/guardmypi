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
#include <thread>
#include <opencv2/dnn.hpp>
#define min_area 500 //pixels

using namespace std;
using namespace cv;
using namespace dnn;

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
	 int i = 0;
	 int closecase();
	 int flag = 0;
};

class HumanDetector {
    public:
        // prepare model files and set up network
        std::string model = "yolov2-tiny.weights";
        std::string config = "yolov2-tiny.cfg";        
        Net network = readNet(model, config,"Darknet"); 
        void configure_network(){
            network.setPreferableBackend(DNN_BACKEND_DEFAULT);
            network.setPreferableTarget(DNN_TARGET_OPENCL);
        }

        Mat detect(Mat img){
            static Mat blobFromImg;
            bool swapRB = true;
            // function which facilitates classification
            blobFromImage(img, blobFromImg, 1, Size(416, 416), Scalar(), swapRB, false);
            //cout << blobFromImg.size() << endl; 
            
            float scale = 1.0 / 255.0;
            Scalar mean = 0;
            network.setInput(blobFromImg, "", scale, mean);

            Mat outMat;
            network.forward(outMat);
            // number of detected objects
            int rowsNoOfDetection = outMat.rows;
            int colsCoordinatesPlusClassScore = outMat.cols;
            // Loop over number of detected objects
            for (int j = 0; j < rowsNoOfDetection; ++j)
            {
                // get classifier scores
                Mat scores = outMat.row(j).colRange(5, colsCoordinatesPlusClassScore);
                Point PositionOfMax;
                double confidence;

                minMaxLoc(scores, 0, &confidence, 0, &PositionOfMax);
                if (confidence > 0.0001)
                {
                    // locate object
                    int centerX = (int)(outMat.at<float>(j, 0) * img.cols); 
                    int centerY = (int)(outMat.at<float>(j, 1) * img.rows); 
                    int width =   (int)(outMat.at<float>(j, 2) * img.cols+20); 
                    int height =   (int)(outMat.at<float>(j, 3) * img.rows+100); 
                    int left = centerX - width / 2;
                    int top = centerY - height / 2;

                    stringstream ss;
                    ss << PositionOfMax.x;
                    string clas = ss.str();
                    putText(img, "Human Detected", Point(left, top), 1, 2, Scalar(0, 0, 255), 2, false);
                    // draw rectangle around object
                    rectangle(img, Rect(left, top, width, height), Scalar(0,0,255), 2, 8, 0);
                }
            } 
			return img;    
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
	 Mat humanframe;
	 VideoCapture video;	//!< Video Input
	 //Mat frame;		//!< Incoming camera feed	

	public:
		 int opencam(Mat frame);		
};

/**
* @class Camera
* @fn opencam
* @brief Opens Camera and transmits video feed
*/

int MotionDetector::closecase() {
	
	while (flag) {
		cout << "Do human/pet detection\t" <<"Iteration :\t "<< i << "\n";
		i++;
		flag = 0;
	}
	return 0;
}

 int Camera::opencam(Mat frame)  {

		//Open the video feed for the webcam/camera
		video.open(0);
		sleep(3);


		//Set width and height of the video feed
		//video.set(CAP_PROP_FRAME_WIDTH,480);
  		//video.set(CAP_PROP_FRAME_HEIGHT, 480);

        // Check that video is opened
	    if (!video.isOpened()) return -1;

		// Loop through available frames
		while (1) {
			
		//Grab the current frame
		video.read(frame);
		detector.ProcessContours(frame);
		humanframe = frame;

		if(detector.flag == 1) {
			thread t2(&HumanDetector::detect, &Hdetector, humanframe);
			t2.join();
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
	Camera camptr;
	MotionDetector detector;
	thread t1(&Camera::opencam, &camptr, cameraframe);
	t1.join();
	//delete camptr;
	return 0;
}
