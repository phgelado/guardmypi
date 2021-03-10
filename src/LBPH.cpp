#include "opencv2/core.hpp"
#include "opencv2/face.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <stdio.h>


using namespace std;
using namespace cv;
using namespace cv::face;

int main() {

Ptr<LBPHFaceRecognizer> recogniser = face::LBPHFaceRecognizer::create();
recogniser->read("test.yml");
CascadeClassifier face_cascade;
face_cascade.load("haarcascade_frontalface_default.xml");
VideoCapture video;   
Mat feed, gray;
int ID = -1;
double confidence = 0.0;
string name;
std::vector<Rect> humans;
video.open(0);


while(1) {

    video.read(feed);

    cvtColor(feed, gray, COLOR_BGR2GRAY);

    
    face_cascade.detectMultiScale(gray, humans, 1.2, 5);

    for( int i = 0; i < humans.size(); i++ )
            {
            Rect r = humans[i];
            Scalar color = Scalar(255, 0, 0);
            rectangle( feed, Point(round(r.x*1), round(r.y*1)), Point(round((r.x + r.width-1)*1), round((r.y + r.height-1)*1)), color, 3, 8, 0);   
            recogniser->predict(gray,ID,confidence);
            if(ID ==0){
                name = "Aidan";
            }
            else
                if(ID==1) {
                    name = "Arnie";
                }
            
            putText(feed,name,Point(10, 20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,0,255),2);            //rectangle(feed, (r.x-22,r.y-90), (r.x+r.width+22, r.y-22), (0,255,0), -1);
            }  
            imshow( "Camera", feed );
               if (waitKey(10) == 27){       
                  break; 
                 }
            }
    video.release();
    destroyAllWindows();
    return 0;
}


