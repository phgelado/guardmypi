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

	///@brief Takes the incoming frame and performs background subtraction using previous frames.
	///Performs background subtraction to detect changes in the frame i.e. motion. 
	///Subsequently, alters the flag variable equal to 1 if motion is detected to invoke the object detector in a new thread
	///@see ObjectDetector::detect Camera:opencam
	///@param camerafeed frame captured from PiCamera or Webcam
	///@returns camerafeed with or without "Motion Detected" text to signify code functioning
Mat MotionDetector::ProcessContours(Mat camerafeed) {
	
	//Convert the input image to grayscale
	cvtColor(camerafeed, grayscale, COLOR_RGB2GRAY);

	//Add a smoother to the image  
	GaussianBlur(grayscale, grayscale, Size(21,21), 0);


	/*	Checks if the avg frame is initialised
	*	If not, set the current frame to avg in the same format as the grayscale image
	*	If the background frame is empty convert the current grayscale image to 32 bit float and store in the avg frame
	*/
	if(avg.empty() == 1) {
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
	threshold(frame_diff, frame_thresh, 30, 255, THRESH_BINARY);
				
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

int ObjectDetector::loadcascade(String cascadename){
            cascade.load(cascadename);
            if(!cascade.load(cascadename))
           {
                cerr<<"Error Loading XML file"<<endl;
            return 0;
           }    
        }

Mat ObjectDetector::detect(Mat ReferenceFrame, double scale_factor, int neighbours){
	// store original frame as grayscale in gray frame
	cvtColor(ReferenceFrame, GrayFrame, COLOR_BGR2GRAY);
	std::vector<Rect> objects;
	// detect objects in frame - adjust parameters as desired
	cascade.detectMultiScale( GrayFrame, objects, scale_factor, neighbours);    

	// Draw rectangles on the detected objects
	for( int i = 0; i < objects.size(); i++ )
	{
		flag = 1;
		Rect rect = objects[i];
			pt1.x = rect.x;					// Origin point of rectangle on the x-axis 
			pt1.y = rect.y;					// Origin point of rectangle on the y-axis 
			pt2.x = rect.x + rect.width;	// Final point along x-axis 
			pt2.y = rect.y + rect.height;	//Final point along y-axis 
	rectangle(ReferenceFrame, pt1,pt2, Scalar(0,255,0), 3, 2, 0);
		}
	return ReferenceFrame; 
}
///@brief Loads the hand and face cascades used to unlock the system during
///daytime hours (7am-8pm)
///@see Unlock::face
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

///@brief This method takes the input frame from the camera if the time
///is between 8am and 9pm. It additionally takes the time stamp called immediately
///before the function. 
///The method looks for a particularly trained face depending on the trained algorithm (*.yml file), which
///if not detected in less than 10s return 0 and set the intruder flag high alerting the user.
///@see Unlock::loadcascade()
///@param ReferenceFrame Input frame from the video frame capture
///@param startTime Time stamp initially called prior to the methd
///@returns camerafeed with or without "Motion Detected" text to signify code functioning

int Unlock::face(Mat ReferenceFrame, clock_t startTime) {

	// store original frame as grayscale in gray frame
    cvtColor(ReferenceFrame, GrayFrame, COLOR_BGR2GRAY);
            
	//!<Initialise Vecotor to store points of detected faces		
	std::vector<Rect> face;

    // detect faces in frame - adjust parameters as desired
	face_cascade.detectMultiScale(GrayFrame, face, 1.3, 8);     
            
	//Resize frame to quicken recognition process
	resize(GrayFrame,GrayFrame,Size(168,192));
	
	//Find the number of seconds passed since the method was initially called
	secondsPassed = (clock() - startTime) / CLOCKS_PER_SEC;

    for( int i = 0; i < face.size(); i++ )	{
        Rect r = face[i];
        area = r.width*r.height;
        Scalar color = Scalar(255, 0, 0);
        recogniser->setThreshold(123);
		
		//Call facial recognition method
		recogniser->predict(GrayFrame,ID,confidence);
		
		if(ID ==0 && secondsPassed < 10){	//0 is the residents face ID
			intruderflag = 0;				//Keep intruder flag 0
            name = "Aidan";					//Set the name to the appropriate resident
			//Put name above the resident's head for confirmation
            putText(ReferenceFrame,"Aidan",Point(round(r.x),round(r.y-5)), FONT_HERSHEY_COMPLEX_SMALL,1,color,2);
			cout << "Welcome home";
			secondsPassed = 0;				//Reset the secondsPassed method
			faceflag = 1;					//Set the facial recognition flag to high
			break;							//Break from the loop
			}
		return 1;
	}	

	if(secondsPassed >= 10 && faceflag !=1) {
		intruderflag = 1;
		return 0;
		}
				
}

int Unlock::QRUnlock(Mat frame, clock_t startTime) {

	secondsPassed = (clock() - startTime) / CLOCKS_PER_SEC;
	//QRcode detection 
	std::string data = qrDecoder.detectAndDecode(frame, bbox, rectifiedImage);
	if(data.length()>0 && secondsPassed < 10) {
		if(data=="user1234"){
			cout << "Valid unlock key detected, decoded data: " << data << endl;
			QRunlockflag = 1;
			return 1;
		}

	else {
    cout << "Invalid unlock key detected,decoded data: " << data << endl;
	return 0;
	
	} 
	 }
	else if (secondsPassed >=10 && QRunlockflag != 1) {
		cout << "In loop:" << secondsPassed;
		cout << "Intruder detected\n";
		intruderflag = 1;
	}
}

int Unlock::QRLock(Mat frame) {

	
  //QRcode detection 
  std::string data = qrDecoder.detectAndDecode(frame, bbox, rectifiedImage);
  if(data.length()>0)
  {

    if(data == "lock" && QRlockflag == 0){
        cout << "System will be locked leave premises now" << data << endl;
        //display(frame, bbox);
        //rectifiedImage.convertTo(rectifiedImage, CV_8UC3);
        //imshow("Rectified QRCode", rectifiedImage);

        //waitKey(0);
		QRlockflag = 1;
		return 1;
		
        //Call unlock function/change flags for unlock
    		} 


	}
	return 0;
}
   



int Camera::lock(int motionflag, int faceflag, int intruderflag,int QRunlockflag, int QRlockflag) {
		motionflag = 0;
		faceflag = 0;
		intruderflag = 0;
		QRunlockflag = 0;
		QRlockflag = 0;
		sleep(5);
	}

int Camera::gettime() {
	time_t ttime = time(0);
    tm *local_time = gmtime(&ttime);
	return int(1 + local_time->tm_hour);
}

int Camera::opencam()  {
		
		//Open the video feed for the webcam/camera
		video.open(0);
		petdetector.loadcascade("haarcascade_dogface.xml");
		recognise.loadcascade();
        video.read(background);
        cvtColor(background, background, COLOR_RGB2GRAY);
        GaussianBlur(background, background, Size(21,21), 0);
        int timerflag = 1;
		clock_t startTime;

        // Check that video is opened
	    if (!video.isOpened()) return -1;
		
		// Loop through available frames
		while (1) {

			
			if(recognise.QRlockflag == 1) {
			cout << "Flag before:" << recognise.intruderflag << "\n";
			cout << "\t" << recognise.QRlockflag << "Leave house lock procedure underway";			
			motiondetector.flag = 0;
			recognise.intruderflag = 0;
			recognise.QRunlockflag = 0;
			recognise.QRlockflag = 0;
			recognise.faceflag = 0;
			cout << "Before:" << recognise.secondsPassed;
			waitKey(5000);
			motiondetector.avg = testframe;
			cout << recognise.avg;
			video.read(frame);
			motiondetector.ProcessContours(frame);
			}

			//Grab the current frame
			video.read(frame);
			//resize(frame,frame,Size(340,200));

		motiondetector.ProcessContours(frame);
		if(motiondetector.flag == 1 && timerflag ==1) {
			startTime = clock();
			timerflag = 0;
		}

		/*
		if(motiondetector.flag == 1) {
			thread t0(&ObjectDetector::detect,&petdetector,frame, 1.3, 20);
			t0.join();
		}
		*/

			// all this  previously under if statement
			hour = gettime();
			
			if (hour >= 7 && hour <= 20 && motiondetector.flag ==1 ) {
				thread t1(&Unlock::face, &recognise, frame, startTime);
				t1.join();
				}  else if (hour < 7 && hour > 20 && motiondetector.flag == 1) {
			 		 thread t1(&Unlock::QRUnlock, &recognise, frame, startTime);
			  	}
			
		if(recognise.QRunlockflag == 1 || recognise.faceflag == 1) {
			motiondetector.flag = 0;
			timerflag = 1;
		}

		if(recognise.faceflag  == 1 || recognise.QRunlockflag == 1) {
			thread t2(&Unlock::QRLock, &recognise, frame);
			t2.join();
			}
		//Show the Video Feed
		imshow("Camera", frame);

	


		if(recognise.intruderflag == 1) {
			cout << "Intruder detected run Notification Thread\n";
		}

		if (waitKey(25) >= 0) break;

		} // end while (video.read(frame))

	
	//Release video capture and write
	video.release(); 

	// Destroy all windows
	destroyAllWindows();
    return 0;
  }