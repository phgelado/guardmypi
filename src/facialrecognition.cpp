// Generic framework for detecting objects using preloaded Haar cascade
// xml files. In this case it will be used for facial recognition for our
// trained model.


#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core.hpp"
#include "opencv2/face.hpp"
#include <fstream>
#include <iostream>
#include <stdio.h>
  
using namespace std;
using namespace cv;
using namespace cv::face;

class FaceDetector{
    public:
        // cascade classifier object
        CascadeClassifier face_cascade;
        Ptr<FisherFaceRecognizer> recogniser = face::FisherFaceRecognizer::create(0,123);
        int ID = -1;
        double confidence = 0.0;
        // grayscale frame for processing
        Mat GrayFrame;
        string name;
        int area;

        // method for loading particular Haar cascade file
        int loadcascade(){
            // insert facial recognition cascade here	
            face_cascade.load("haarcascade_frontalface_default.xml");
            // and here
            
            recogniser->read("eigensmall.yml");

            if(!face_cascade.load("haarcascade_frontalface_default.xml"))
           {
                cout<<"Error Loading XML file"<<endl;
            return 0;
           }    
        }

        // method for detecting humans in frame
        void detect(Mat ReferenceFrame){
            // store original frame as grayscale in gray frame
            cvtColor(ReferenceFrame, GrayFrame, COLOR_BGR2GRAY);
            std::vector<Rect> humans;
            // detect humans in frame - adjust parameters as desired
            face_cascade.detectMultiScale(GrayFrame, humans, 1.1, 8);     
            resize(GrayFrame,GrayFrame,Size(168,192));
            // Draw rectangles on the detected humans
            for( int i = 0; i < humans.size(); i++ )
            {
            Rect r = humans[i];
            area = r.width*r.height;
            Scalar color = Scalar(255, 0, 0);

            if(area > 2000) {
                rectangle( ReferenceFrame, Point(round(r.x*1), round(r.y*1)), Point(round((r.x +
                r.width-1)*1), round((r.y + r.height-1)*1)), color, 3, 2, 0);
            }     
            recogniser->predict(GrayFrame,ID,confidence);
            //name = to_string(ID);
            string conf = to_string(confidence);
            cout << ID << "\t" << confidence << "\n";
            putText(ReferenceFrame,conf,Point((10,20)), FONT_HERSHEY_COMPLEX_SMALL,1,color,2);


            if(ID ==11 && confidence < 11000){
                name = "Aidan";
                putText(ReferenceFrame,"Aidan",Point(round(r.x),round(r.y-5)), FONT_HERSHEY_COMPLEX_SMALL,1,color,2);

            }
            
                //putText(ReferenceFrame, name,Point(10, 20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,0,255),2);
                //putText(ReferenceFrame, conf,Point(30, 20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,0,255),2);


            }
        }
};

// camera class
class Camera: VideoCapture{
  public:
      FaceDetector mydetector;
      Mat ReferenceFrame;



      // start camera method
      int start(){
            // initialise capture
            VideoCapture capture(0);   
            // load cascade here
            mydetector.loadcascade();   
            //if (!capture.isOpened()) return -1;
            while(1){
              //ReferenceFrame =imread("aidantest.JPG");
              // get next frame

               capture >> ReferenceFrame;
               mydetector.detect(ReferenceFrame);
               if( ReferenceFrame.empty() )
                  break;
               // show image in window 
               resize(ReferenceFrame,ReferenceFrame,Size(720,480));
               imshow( "Camera", ReferenceFrame );
               if (waitKey(10) == 27){       
                  break;
               }   
            } 
          return 0;
      }                   
};
  
int main()
{
    Camera mycamera;
    mycamera.start();
}
