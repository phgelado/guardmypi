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
	///@see ObjectDetector::detect Camera::opencam
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

///@brief Loads the classifier with a given cascade file
///@param cascadename File name of cascade XML file
int ObjectDetector::loadcascade(String cascadename){
	cascade.load(cascadename);
	if(!cascade.load(cascadename))
	{
		cerr<<"Error Loading XML file"<<endl;
	return 0;
	}    
}

///@brief Performs object detection on the incoming frame for the given cascade file. If the object (in this case a pet)
/// is detected within 10 seconds, a flag is raised. If not, the flag is set to 0.
///@param ReferenceFrame Input frame from the video frame capture
///@param scale_factor Parameter given to detectMultiScale method, specifying how much the image size is reduced at each image scale
///@param neighbours Parameter given to detectMultiScale method, specifying how many neighbors each candidate rectangle should have to retain it
///@param startTime Initial time stamp used to calculate time passed
int ObjectDetector::detect(Mat ReferenceFrame, double scale_factor, int neighbours,clock_t startTime){
	// store original frame as grayscale in gray frame
	cvtColor(ReferenceFrame, GrayFrame, COLOR_BGR2GRAY);
	std::vector<Rect> objects;
	// detect objects in frame - adjust parameters as desired
	cascade.detectMultiScale( GrayFrame, objects, scale_factor, neighbours);    
	
	int secondsPassed = (clock() - startTime) /CLOCKS_PER_SEC;
	//cout << secondsPassed << "\t" << flag << "\n";
	for( int i = 0; i < 1000; i++ )
	{
		if(objects.size() >=1 && secondsPassed< 10){
			flag = 1;
			putText(ReferenceFrame, "Pet Detected", Point(110, 20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,0,255),2);
			break;
		}

		if (secondsPassed>=10 && objects.size() == 0){
			flag = 0;
			break;
		}		 
		
	}
}

///@brief Loads the hand and face cascades used to unlock the system during
///daytime hours (7am-8pm)
///@see Unlock::face
int Unlock::loadcascade(){
	face_cascade.load("src/haarcascade_frontalface_default.xml");
	recogniser->read("src/guardingthepi.yml");

	if(!face_cascade.load("src/haarcascade_frontalface_default.xml"))
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
            
	//Initialise Vecotor to store points of detected faces		
	std::vector<Rect> face;
	

    // detect faces in frame - adjust parameters as desired
	face_cascade.detectMultiScale(GrayFrame, face, 1.3, 8);     
        resize(GrayFrame,GrayFrame, Size(168,192));


	
	//Find the number of seconds passed since the method was initially called
	secondsPassed = (clock() - startTime) / CLOCKS_PER_SEC;
	//cout << secondsPassed << "\t" << faceflag << "\t" << intruderflag << "\n";
	cout << intruderflag << "\n";
    for( int i = 0; i < face.size(); i++ )	{
        Rect r = face[i];
        Scalar color = Scalar(255, 0, 0);
      
		recogniser->setThreshold(123);
		//Call facial recognition method
		recogniser->setThreshold(123);
		recogniser->predict(GrayFrame,ID,confidence);
		if(ID ==0 && secondsPassed < 10){	//0 is the residents face ID
			intruderflag = 0;				//Keep intruder flag 0
            name = "Aidan";					//Set the name to the appropriate resident
			//Put name above the resident's head for confirmation
            putText(ReferenceFrame,"Aidan",Point(round(r.x),round(r.y-5)), FONT_HERSHEY_COMPLEX_SMALL,1,color,2);
			//cout << "Welcome home";
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
		// Break from the function
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

		if(data=="unlock"){			//QR code must equate unlock
		//	cout << "Valid unlock key detected, decoded data: " << data << endl;
			//Set QR flag high so system knows that it is in the unlocked state
			QRunlockflag = 1;		
			return 1;		//Break from code
		}

		// Condition is added incase the wrong QR Code has been shown
		else {  
    	//	cout << "Invalid unlock key detected,decoded data: " << data << endl;
			return 0;
		} 
	}
	// Condition is met if the timer has expired and the QR code has not been presented
	else if (secondsPassed >=10 && QRunlockflag != 1) {		
		//Set the intruder flag high to alert the user and break from the function
		//cout << "Intruder detected";
		intruderflag = 1;
		return 0;
	}
}

///@brief This method searches for the user to reset the system when it's detected an intruder.
///By essentially setting the intruder flag to 0 and all unlock method flags to high the system enters it's disarmed state
///@param frame Input frame from the camera feed
///@returns 1 to escape function once QR code is detected
///@returns 0 to escape function if the incorrect/no QR code is detected
int Unlock::Deactivate(Mat frame) {
	//QRcode detection 
	std::string data = qrDecoder.detectAndDecode(frame, bbox, rectifiedImage);
	
	if(data.length()>0) { //data length is > 0 if it has read a QR code

		if(data=="unlock"){			//QR code must equate unlock
		//	cout << "Valid unlock key detected, decoded data: " << data << endl;
			//Set QR flag high so system knows that it is in the unlocked state
			QRunlockflag = 1;	
			faceflag = 1;
			intruderflag = 0;	
			return 1;		//Break from code
		}

		// Condition is added incase the wrong QR Code has been shown
		else {  
    	//	cout << "Invalid unlock key detected,decoded data: " << data << endl;
			return 0;
		} 
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

	//Conditional statement looking for a QR Code called lock
	if(data == "lock" && QRlockflag == 0){

       // cout << "System will be locked leave premises now" << data << endl;
  		//Set lock flag high to engage the system re-arming state letting the user leave etc...
		QRlockflag = 1;
		return 1;  //Break from function
    		} 
	}
	return 0;  //Nothing happening so jjust break from function and return 0
}

///@brief Calculates the current hour of the day to ensure the system knows what method 
///of unlocking the system to use
///@see Unlock::face() Unlock::QRUnlock()
///@returns The current hour of the day for GMT zone
int Camera::gettime() {
	time_t ttime = time(0);	//Time since January 1st 1990
    tm *local_time = gmtime(&ttime);	//Get GMT 
	return int(1 + local_time->tm_hour);	// Return the hour of the day
}

///@brief Opens the camera in the main thread and runs an infinite loop of frame capturing.
///Each frame is processed by various methods to detect motion, facial recognition and QR recognition.
///@see Unlock::face() Unlock::QRUnlock Unlock::QRLock MotionDetector::ProcessContours()
int Camera::opencam(int camport)  {
		
	//Open the video feed for the webcam/camera
	video.open(camport);

	// Run all the loadcascade methods for each detection algorithm
	petdetector.loadcascade("src/haarcascade_dogface.xml");
	recognise.loadcascade();

	//Set timerflags high 
	pet_timerflag = 1;
	recognise_timerflag = 1;
	int frame_width = static_cast<int>(video.get(CAP_PROP_FRAME_WIDTH)); //get the width of frames of the video
	int frame_height = static_cast<int>(video.get(CAP_PROP_FRAME_HEIGHT)); //get the height of frames of the video
	Size frame_size(frame_width, frame_height);
    int frames_per_second = 10;
    int iterationflag = 1;

    //Create and initialize the VideoWriter object 
   	VideoWriter oVideoWriter("MyVideo.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), frames_per_second, frame_size, true); 

	// Check that video is opened
	if (!video.isOpened()) return -1;
	
	//system("xterm -hold -e 'sudo LD_LIBRARY_PATH=/home/pi/mjpg-streamer/mjpg-streamer-experimental mjpg_streamer -i \"input_file.so -f /home/pi/guardmypi -n test.jpg\" -o \"output_http.so -w /home/pi/mjpg-streamer/mjpg-streamer-experimental/www -p 80\"' & ");
	// Loop through available frames 
	while (1) {
		
		//Grab the current frame
		video.read(frame);
		
		if(recognise.QRlockflag == 1) {
			//Reset all the required flags to rearm the system	
			//petdetector.flag = -1;
			motiondetector.flag == 1;
			recognise.intruderflag = 0;
			recognise.QRunlockflag = 0;
			recognise.QRlockflag = 0;
			recognise.faceflag = 0;
			pet_timerflag = 1;
			emailflag = 1;
			iterationflag =1;
			recognise_timerflag = 1;
			recognise.intruderflag = 0;
			motiondetector.avg = testframe;
			
			//Wait 60s for the user to leave the house before activating the system
			waitKey(5000);
			// Empty the running average frame for the motion detector by assigning an empty frame
			/*
			motiondetector.avg = testframe;
			video.read(frame);
			motiondetector.ProcessContours(frame);
			motiondetector.flag = 0;
			*/
			
			for (int j = 0; j < 5; j++) {
			motiondetector.flag = 0;
			motiondetector.avg = testframe;
			video.read(frame);
			motiondetector.ProcessContours(frame);
			motiondetector.flag = 0;
		}	
			motiondetector.avg = testframe;
			video.read(frame);
			motiondetector.flag = 0;
			}

		//Call the motion detector method and supply the input frame
		if(motiondetector.flag == 0) {
		motiondetector.ProcessContours(frame);
		//cout << "Motion Detection:" << motiondetector.flag;

		}
		//cout << motiondetector.flag;
/*
		//If motion is detected start the timer...
		if(motiondetector.flag == 1 && pet_timerflag ==1) {
			pet_startTime = clock();
			pet_timerflag = 0;
		}
		*/
 /*
		if (motiondetector.flag ==1){
				thread t0(&ObjectDetector::detect,&petdetector,frame, 1.3, 50,pet_startTime);
				t0.join();
		}
		* */
/*
		// if a pet is detected reset the flags i.e. go back to normal
		if (petdetector.flag ==1){
			motiondetector.flag = 0;
			pet_timerflag = 1;
			petdetector.flag = -1;
		}
*/
		if (recognise_timerflag ==1 && motiondetector.flag == 1){
			recognise_startTime = clock();
			recognise_timerflag = 0;

			//motiondetector.flag = 0;
		}

		//Check the time to run the appropriate unlocking method
		hour = gettime();
		//If the time is between 7am and 8pm then run facial recognition
		if (hour >= 7 && hour <= 20 && motiondetector.flag == 1 && iterationflag ==1) {
			//Create a thread to break off from main and run the facial recognition
			thread t1(&Unlock::face, &recognise, frame, recognise_startTime);	
			t1.join();
			
		}  
		//If it is night time the system struggles to identify faces so invoke QR Unlocking
		else if (hour < 7 && hour > 20 && motiondetector.flag == 1 && iterationflag == 1) {
					//Break offf from main and run the QR Code in a seperate thread
					thread t1(&Unlock::QRUnlock, &recognise, frame, recognise_startTime);
					t1.join();
					
		}
		/*
		*If either unlocking method has been satisfied then reset the motionflag and timerflag.
		*This essentially resets the timer, then calls the Locking method in a seperate thread from main
		*The system is currently in the unlocked state if this condition is met
		*/
		if(recognise.faceflag  == 1 || recognise.QRunlockflag == 1) {
			//petdetector.flag = -1;
			iterationflag = 0;
			recognise_timerflag = 1;
			thread t2(&Unlock::QRLock, &recognise, frame);
			t2.join();
		}

		

		/*If an intruder is detected in the home... */

		//...send an email to the user...
		if (recognise.intruderflag ==1 && emailflag == 1){
			cout << "Intruder Detected";
			system("sudo echo \"A possible intruder has been detected in your home. Please check http://guardmypi.com/ for remote streaming.\" | mail -s \"Possible intruder detected\" magnusbc98@gmail.com");
			emailflag = 0;
			iterationflag = 0;
		}
		//... begin writing the video footage to a .avi file...
		if(recognise.intruderflag == 1) {
			oVideoWriter.write(frame); 
			// ...continually save the frames into a jpg file which will then be
			// streamed to the website.
			imwrite("test.jpg", frame); 
			sleep(0.005);
			recognise_timerflag = 1;
			//petdetector.flag = -1;
			thread t1(&Unlock::Deactivate, &recognise, frame);
			t1.join();
					
		
			//Check the frame is not empty
			if(frame.empty())	{
			  	std::cerr << "Something is wrong with the webcam, could not get frame." << std::endl;
			}
		/* End of intruder code */
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
