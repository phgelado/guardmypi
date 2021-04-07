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

	//After 10s the intruder flag is set high if the resident's face is not detected
	if(secondsPassed >= 10 && faceflag !=1) {
		//Set intruder flag high to then alert the user!
		intruderflag = 1;	
		//Break from the function
		return 0;	
		}
}

///@brief  This method is the alternative unlocking method once it is night time
///@see Unlock::loadcascade() Unlock::face()
///@param frame Input frame from the video frame capture
///@param startTime Time stamp initially called prior to the methd
///@returns 1 to escape function once QR code is detected
///@returns 0 to escape function if the incorrect/no QR code is detected
int Unlock::QRUnlock(Mat frame, clock_t startTime) {

	//Calculate the number of seconds passed
	secondsPassed = (clock() - startTime) / CLOCKS_PER_SEC;

	//QRcode detection 
	std::string data = qrDecoder.detectAndDecode(frame, bbox, rectifiedImage);
	
	if(data.length()>0 && secondsPassed < 10) { //data length is > 0 if it has read a QR code

		if(data=="unlock"){			//!<QR code must equate unlock
			cout << "Valid unlock key detected, decoded data: " << data << endl;
			//!<Set QR flag high so system knows that it is in the unlocked state
			QRunlockflag = 1;		
			return 1;		//Break from code
		}

		//!< Condition is added incase the wrong QR Code has been shown
		else {  
    		cout << "Invalid unlock key detected,decoded data: " << data << endl;
			return 0;
		} 
	}
	//!< Condition is met if the timer has expired and the QR code has not been presented
	else if (secondsPassed >=10 && QRunlockflag != 1) {		
		//!<Set the intruder flag high to alert the user and break from the function
		intruderflag = 1;
		return 0;
	}
}

///@brief This method looks for another QR code whilst the system is in its unlocked 
///state. Until the camera detects a QR code with a key called "lock" then nothing happens.
///@param frame Input frame from the camera feed
///@param startTime Time stamp initially called prior to the methd
///@returns 1 to escape function once QR code is detected
///@returns 0 to escape function if the incorrect/no QR code is detected
int Unlock::QRLock(Mat frame) {

  //QRcode detection 
  std::string data = qrDecoder.detectAndDecode(frame, bbox, rectifiedImage);
  
  //!Condition looking for a QR Code
  if(data.length()>0)	{

	//!<Conditional statement looking for a QR Code called lock
	if(data == "lock" && QRlockflag == 0){

        cout << "System will be locked leave premises now" << data << endl;
  		//!<Set lock flag high to engage the system re-arming state letting the user leave etc...
		QRlockflag = 1;
		return 1;  //Break from function
    		} 
	}
	return 0;  //!<Nothing happening so jjust break from function and return 0
}

///@brief Calculates the current hour of the day to ensure the system knows what method 
///of unlocking the system to use
///@see Unlock::face() Unlock::QRUnlock()
///@returns The current hour of the day for GMT zone
int Camera::gettime() {
	time_t ttime = time(0);	//!<Time since January 1st 1990
    tm *local_time = gmtime(&ttime);	//!<Get GMT 
	return int(1 + local_time->tm_hour);	//!< Return the hour of the day
}

///@brief Opens the camera in the main thread and runs an infinite loop of frame capturing.
///Each frame is processed by various methods to detect motion, facial recognition and QR recognition.
///@see Unlock::face() Unlock::QRUnlock Unlock::QRLock MotionDetector::ProcessContours()
int Camera::opencam()  {
		
	//Open the video feed for the webcam/camera
	video.open(0);

	//!< Run all the loadcascade methods for each detection algorithm
	petdetector.loadcascade("haarcascade_dogface.xml");
	recognise.loadcascade();

	//!<Set timerflag high 
	int timerflag = 1;

	// Check that video is opened
	if (!video.isOpened()) return -1;
	
	// Loop through available frames 
	while (1) {
		
		//Grab the current frame
		video.read(frame);

		//!<Call the motion detector method and supply the input frame
		motiondetector.ProcessContours(frame);

		//!<If motion is detected start the timer...
		if(motiondetector.flag == 1 && timerflag ==1) {
			startTime = clock();
			timerflag = 0;
		}

		//Check the time to run the appropriate unlocking method
		hour = gettime();
		
		//!<If the time is between 7am and 8pm then run facial recognition
		if (hour >= 7 && hour <= 20 && motiondetector.flag ==1 ) {
			//!<Create a thread to break off from main and run the facial recognition
			thread t1(&Unlock::face, &recognise, frame, startTime);	
			t1.join();
			}  
			//!<If it is night time the system struggles to identify faces so invoke QR Unlocking
			else if (hour < 7 && hour > 20 && motiondetector.flag == 1) {
					//!<Break off from main and run the QR Code in a seperate thread
					thread t1(&Unlock::QRUnlock, &recognise, frame, startTime);
			}
		/*
		*If either unlocking method has been satisfied then reset the motionflag and timerflag.
		*This essentially resets the timer, then calls the Locking method in a seperate thread from main
		*The system is currently in the unlocked state if this condition is met
		*/
		if(recognise.faceflag  == 1 || recognise.QRunlockflag == 1) {
			motiondetector.flag = 0;
			timerflag = 1;
			thread t2(&Unlock::QRLock, &recognise, frame);
			t2.join();
		}

		if(recognise.QRlockflag == 1) {
			//!<Reset all the required flags to rearm the system	
			motiondetector.flag = 0;
			recognise.intruderflag = 0;
			recognise.QRunlockflag = 0;
			recognise.QRlockflag = 0;
			recognise.faceflag = 0;

			//!<Wait 60s for the user to leave the house before activating the system
			waitKey(5000);

			//!< Empty the running average frame for the motion detector by assigning an empty frame
			motiondetector.avg = testframe;
		}

		/*
		*If any of the unlocking methods are not satisfied then this flag is set high
		*The user will then be alerted and a casting of the current frames will be sent to the user
		*/
		if(recognise.intruderflag == 1) {
		
		//!<Check the frame is not empty
		if(frame.empty())	{
			  std::cerr << "Something is wrong with the webcam, could not get frame." << std::endl;
			}

			// Save the frame into a file capturing a pic of the intruder
			imwrite("test.jpg", frame); 
			}

		//Show the Video Feed
		imshow("Camera", frame);
	

	if (waitKey(25) >= 0) break;

	} // end while (video.read(frame))


//Release video capture and write
video.release(); 

// Destroy all windows
destroyAllWindows();
return 0;
}