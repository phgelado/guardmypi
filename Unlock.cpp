#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/face.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>

using namespace std;
using namespace cv;
using namespace cv::face;


class Unlock {
    protected: 
        Mat avg,gray,thresh, new_avg, diff;
        CascadeClassifier hand_cascade;
        std::vector<Rect> handvec;
        Scalar color = Scalar(255, 0, 0);
        Point pt1;	//!<Start point/coordinate for the contour rectangle
	Point pt2;	//!<End point/coordinate for the contour rectangle
    
    public:
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

            //else
             //   gray.convertTo(background, CV_32FC(gray.channels()));

            //Find weighted average between the current frame and background.
	       // accumulateWeighted(gray, avg, 0.5);

	        //Convert from Float to 8-bit absolute value
	        //wont allow convertScaleAbs(avg, avg)
	        convertScaleAbs(background, new_avg);

	       //Calculate the absolute difference between the current image and the background image
	        absdiff(gray, new_avg, diff);

	        //Threshold image for differences between the two frames.
	        threshold(diff, thresh, 50, 255, THRESH_BINARY);
				
	        //Dilate the threshold image
	        dilate(thresh, thresh, Mat(), Point(-1,-1), 2); 
            hand_cascade.detectMultiScale(thresh, handvec, 1.1, 8);     

            for( int i = 0; i < handvec.size(); i++)
            {
                Rect rect = handvec[i];
                pt1.x = rect.x;					// Origin point of rectangle on the x-axis 
			    pt1.y = rect.y;					// Origin point of rectangle on the y-axis 
			    pt2.x = rect.x + rect.width;	// Final point along x-axis 
			    pt2.y = rect.y + rect.height;	//Final point along y-axis 
                rectangle(ReferenceFrame, pt1,pt2, color, 3, 2, 0);
                }    

        return thresh;
          
        }
};

class Camera {
	protected:
	Unlock ulock; //!< Instance of the Motion Detection
	VideoCapture video;	//!< Video Input
	Mat frame, threshframe;		//!< Incoming camera feed	
    Mat background;

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
		//video.set(CAP_PROP_FRAME_WIDTH,480);
  		//video.set(CAP_PROP_FRAME_HEIGHT, 480);
		int frame_width=  720;
            	int frame_height= 480;
            	int codec = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');
            	VideoWriter cap("hand.avi",codec,10, Size(frame_width,frame_height),true);

        ulock.loadcascade();
        video.read(background);
        cvtColor(background, background, COLOR_RGB2GRAY);
        GaussianBlur(background, background, Size(21,21), 0);

    // Loop through available frames
		while (1) {
			
		//Grab the current frame
		video.read(frame);

		threshframe = ulock.hand(frame, background);
       	
		//Show the Video Feed
		imshow("Camera", frame);
        	imshow("Threshold", threshframe);
		resize(frame,frame, Size(720,480));
		cap.write(frame);

		// For breaking the loop
		if (waitKey(25) >= 0) break;

		} // end while (video.read(frame))
        
	//Release video capture and write
	video.release(); 
	cap.release();
	// Destroy all windows
	destroyAllWindows();
    return 0;
  }

  
int main()
{
    Camera mycamera;
    mycamera.opencam();
}
