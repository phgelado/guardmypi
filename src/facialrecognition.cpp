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
        Ptr<LBPHFaceRecognizer> recogniser = face::LBPHFaceRecognizer::create();
        int ID = -1;
        double confidence = 0.0;
        // grayscale frame for processing
        Mat GrayFrame;
        string name;

        // method for loading particular Haar cascade file
        int loadcascade(){
            // insert facial recognition cascade here	
            face_cascade.load("haarcascade_frontalface_default.xml");
            // and here
            recogniser->read("test.yml");

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
            face_cascade.detectMultiScale( GrayFrame, humans, 1.5, 2, 0, Size(30,30), Size(240,240));     
  
            // Draw rectangles on the detected humans
            for( int i = 0; i < humans.size(); i++ )
            {
            Rect r = humans[i];
            Scalar color = Scalar(255, 0, 0);
            rectangle( ReferenceFrame, Point(round(r.x*1), round(r.y*1)), Point(round((r.x +
            r.width-1)*1), round((r.y + r.height-1)*1)), color, 3, 8, 0);
            recogniser->predict(GrayFrame,ID,confidence);
            if(ID ==0){
                name = "Aidan";
                putText(ReferenceFrame,name,Point(10, 20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,0,255),2);    

            }
            else
                if(ID==1) {
                    name = "Arnie";
                    putText(ReferenceFrame,name,Point(40, 20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,0,255),2);    
                } 
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
            if (!capture.isOpened()) return -1;
            while(1){
              // get next frame
               capture >> ReferenceFrame;
               mydetector.detect(ReferenceFrame);
               if( ReferenceFrame.empty() )
                  break;
               // show image in window 
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
