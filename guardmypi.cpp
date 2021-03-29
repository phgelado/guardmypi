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
#include "guardmypi.h"

using namespace std;
using namespace cv;
using namespace cv::face;

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

int HumanDetector::loadcascade(){
            human_cascade.load("src/haarcascade_upperbody.xml");
            if(!human_cascade.load("src/haarcascade_upperbody.xml"))
           {
                cerr<<"Error Loading XML file"<<endl;
            return 0;
           }    
        }

Mat HumanDetector::detect(Mat ReferenceFrame){
            // store original frame as grayscale in gray frame
            cvtColor(ReferenceFrame, GrayFrame, COLOR_BGR2GRAY);
            std::vector<Rect> humans;
            // detect humans in frame - adjust parameters as desired
            human_cascade.detectMultiScale( GrayFrame, humans, 1.5, 2);     
  
            // Draw rectangles on the detected humans
            for( int i = 0; i < humans.size(); i++ )
            {
			 flag = 1;
			 Rect rect = humans[i];
                	pt1.x = rect.x;					// Origin point of rectangle on the x-axis 
			    	pt1.y = rect.y;					// Origin point of rectangle on the y-axis 
			    	pt2.x = rect.x + rect.width;	// Final point along x-axis 
			    	pt2.y = rect.y + rect.height;	//Final point along y-axis 
            rectangle(ReferenceFrame, pt1,pt2, Scalar(0,255,0), 3, 2, 0);
			 }
			return ReferenceFrame; 
        }

int Unlock::loadcascade(){
            hand_cascade.load("Hand_haar_cascade.xml");
			face_cascade.load("haarcascade_frontalface_default.xml");
			recogniser->read("guardingthepi.yml");

            if(!hand_cascade.load("Hand_haar_cascade.xml"))
           {
                cout<<"Error Loading XML file"<<endl;
            return 0;
           }    
        }

Mat Unlock::face(Mat ReferenceFrame, clock_t startTime) {
	// store original frame as grayscale in gray frame
            cvtColor(ReferenceFrame, GrayFrame, COLOR_BGR2GRAY);
            std::vector<Rect> face;
            // detect humans in frame - adjust parameters as desired
            face_cascade.detectMultiScale(GrayFrame, face, 1.3, 8);     
            resize(GrayFrame,GrayFrame,Size(168,192));
            // Draw rectangles on the detected humans
            for( int i = 0; i < face.size(); i++ )
            {
            	Rect r = face[i];
            	area = r.width*r.height;
            	Scalar color = Scalar(255, 0, 0);
            	recogniser->setThreshold(123);
				
				
				secondsPassed = (clock() - startTime) / CLOCKS_PER_SEC;
				

				while(secondsPassed < 10 || ID != 0){		
					recogniser->predict(GrayFrame,ID,confidence);
					if(ID ==0){
						intruderflag = 0;
                		name = "Aidan";
                		putText(ReferenceFrame,"Aidan",Point(round(r.x),round(r.y-5)), FONT_HERSHEY_COMPLEX_SMALL,1,color,2);
						faceflag = 1;
						break;
           	 		}	
				} 
					
				if(secondsPassed >= 10) {
					intruderflag = 1;
					cout << "Intruder";
				}
			
            
                //putText(ReferenceFrame, name,Point(10, 20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,0,255),2);
                //putText(ReferenceFrame, conf,Point(30, 20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,0,255),2);
				 }
				 return ReferenceFrame;
}
    



Mat Unlock::hand(Mat ReferenceFrame, Mat background) {
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
        	hand_cascade.detectMultiScale(thresh, handvec, 1.3, 8);  

	/*		
	///////////////////////////////////////////////////////////////////////////////
	//				Execute timer that waits X seconds for a hand detection
	//////////////////////////////////////////////////////////////////////////////
			//Timer implementation NEEDS FIXED only returns 0s 
			if(handvec.size() < 1 && secondsPassed < 2) {
				}

	/////////////////////////////////////////////////////////////////////////////
	//	Execute this if no hand or QR code is detected aka the intruder push notification stuff
	/////////////////////////////////////////////////////////////////////////////
			else if(secondsPassed >= 2) {
				cout << "INTRUDER" << "\n";
				//Escape the function here and set an intruder flag? break; or return 0 to terminate the function then do t2.join()?
			}
			
			else if(handvec.size() >= 1) {
				handflag = 1;
			}

			*/

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



int Lock::lock(int lockflag, int humanflag, int motionflag) {
	if(lockflag == 0) {
		return 0;
	}
	else {
		humanflag = 0;
		motionflag = 0;
		sleep(60);
	}
}

int Camera::opencam()  {

		//Open the video feed for the webcam/camera
		video.open(0);
		recognise.loadcascade();
        video.read(background);
        cvtColor(background, background, COLOR_RGB2GRAY);
        GaussianBlur(background, background, Size(21,21), 0);
        int timerflag = 1;

        // Check that video is opened
	    if (!video.isOpened()) return -1;
		
		// Loop through available frames
		while (1) {
			
		//Grab the current frame
		video.read(frame);
		//resize(frame,frame,Size(340,200));


		detector.ProcessContours(frame);
		if(detector.flag == 1) && (timerflag==1) {
			clock_t startTime;
			timerflag = 0;
		}
		if(detector.flag == 1) {
			thread t1(&Unlock::face, &recognise, frame, startTime);
			t1.join();
		}

		if (recognise.faceflag == 1) {
			thread t2(&Unlock::hand, &recognise, frame, background);
			t2.join();
		}
		//cout << "ID:" << recognise.ID << "\t Flag:" << recognise.faceflag;
		/*
		if(recognise.flag == 1) {
			//Put code in here that looks for something to lock the system it will set lockflag == 1
		}
		*/

		//locksystem.lock(flag, Hdetector.flag, detector.flag);

		//Show the Video Feed
		imshow("Camera", frame);


		//Reset function probably gonna go in here. Code only works properly when I put flag resets here!
		detector.flag = 0;
		recognise.faceflag=0;
		// For breaking the loop
		if (waitKey(25) >= 0) break;

		} // end while (video.read(frame))
    
	
	//Release video capture and write
	video.release(); 

	// Destroy all windows
	destroyAllWindows();
    return 0;
  }
