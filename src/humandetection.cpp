// Generic framework for detecting objects using preloaded Haar cascade
// xml files. In this case it will be used for facial recognition for our
// trained model.


#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
  
#include <iostream>
#include <stdio.h>
  
using namespace std;
using namespace cv;

class HumanDetector{
    public:
        // cascade classifier object
        CascadeClassifier hua_cascade;
        // grayscale frame for processing
        Mat GrayFrame;

        // method for loading particular Haar cascade file
        int loadcascade(){
            human_cascade.load("haarcascade_upperbody.xml");
            if(!face_cascade.load("haarcascade_upperbody.xml"))
           {
                cerr<<"Error Loading XML file"<<endl;
            return 0;
           }    
        }

        // method for detecting humans in frame
        void detect(Mat ReferenceFrame){
            // store original frame as grayscale in gray frame
            cvtColor(ReferenceFrame, GrayFrame, COLOR_BGR2GRAY);
            std::vector<Rect> humans;
            // detect humans in frame - adjust parameters as desired
            face_cascade.detectMultiScale( GrayFrame, humans, 1.5, 2);     
  
            // Draw rectangles on the detected humans
            for( int i = 0; i < humans.size(); i++ )
            {
             rectangle(ReferenceFrame, humans[i], Scalar(0,255,0));
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
